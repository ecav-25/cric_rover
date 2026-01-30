// ============================================================================
// AIEngine.cpp - IMPLEMENTAZIONE
// ============================================================================

#include <Arduino.h>
#include "AIEngine.h"

// Dichiarazioni esterne da model.h (definite nel file generato)
extern const unsigned char model_data[];
extern const float input_min[];
extern const float input_max[];

AIEngine::AIEngine()
    : model(nullptr), interpreter(nullptr), input(nullptr), output(nullptr),
      currentMSE(0.0f), alarmActive(false), anomalyCounter(0),
      lastInferenceMs(0), inferenceCount(0) {
    memset(historyBuffer, 0, sizeof(historyBuffer));
    memset(inputBackup, 0, sizeof(inputBackup));
}

bool AIEngine::initialize() {
    AI_DEBUG("Initializing TensorFlow Lite model...");
    
    model = tflite::GetModel(model_data);
    if (!model) {
        AI_DEBUG("❌ Failed to load model!");
        return false;
    }
    
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        AI_DEBUG("❌ Model version mismatch!");
        return false;
    }

    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensorArena, AIConfig::TENSOR_ARENA_SIZE, &errorReporter
    );
    interpreter = &static_interpreter;

    if (interpreter->AllocateTensors() != kTfLiteOk) {
        AI_DEBUG("❌ AllocateTensors failed!");
        return false;
    }

    input = interpreter->input(0);
    output = interpreter->output(0);
    
    if (!input || !output) {
        AI_DEBUG("❌ Input or output tensor is NULL!");
        return false;
    }

    AI_DEBUG("✓ Model initialized successfully");
    return true;
}

float AIEngine::normalizeFeature(float raw_value, int feature_idx) {
    float range = input_max[feature_idx] - input_min[feature_idx];
    
    if (range <= AIConfig::MIN_RANGE) {
        return 0.5f;
    }
    
    float normalized = (raw_value - input_min[feature_idx]) / range;
    return constrain(normalized, 0.0f, 1.0f);
}

void AIEngine::processData(const RoverTelemetry& telemetry) {
    // Estrai features grezze
    float rawFeatures[AIConfig::NUM_FEATURES] = {
        (float)telemetry.rpm_fl,
        (float)telemetry.rpm_fr,
        (float)telemetry.rpm_rl,
        (float)telemetry.rpm_rr,
        (float)telemetry.target_fl,
        (float)telemetry.target_fr,
        (float)telemetry.target_rl,
        (float)telemetry.target_rr
    };

    // Sliding window: sposta i dati precedenti
    for (int t = 0; t < AIConfig::TIME_STEPS - 1; t++) {
        for (int f = 0; f < AIConfig::NUM_FEATURES; f++) {
            historyBuffer[t][f] = historyBuffer[t + 1][f];
        }
    }

    // Normalizza e inserisci i dati attuali
    for (int i = 0; i < AIConfig::NUM_FEATURES; i++) {
        historyBuffer[AIConfig::TIME_STEPS - 1][i] = normalizeFeature(rawFeatures[i], i);
    }

    // Attendi che il buffer sia pieno
    if (inferenceCount < AIConfig::TIME_STEPS) {
        inferenceCount++;
        return;
    }

    // Carica il tensore di input
    for (int t = 0; t < AIConfig::TIME_STEPS; t++) {
        for (int f = 0; f < AIConfig::NUM_FEATURES; f++) {
            int idx = (t * AIConfig::NUM_FEATURES) + f;
            input->data.f[idx] = historyBuffer[t][f];
            inputBackup[idx] = historyBuffer[t][f];
        }
    }

    // Esegui inferenza
    if (interpreter->Invoke() != kTfLiteOk) {
        AI_DEBUG("❌ Inference failed!");
        return;
    }

    // Calcola MSE (Mean Squared Error)
    float totalLoss = 0.0f;
    for (int i = 0; i < AIConfig::TOTAL_INPUTS; i++) {
        float diff = inputBackup[i] - output->data.f[i];
        totalLoss += (diff * diff);
    }
    float mse = totalLoss / AIConfig::TOTAL_INPUTS;
    // Aggiorna stato allarme
    updateAlarmState(mse);
}

void AIEngine::updateAlarmState(float mse) {
    portENTER_CRITICAL(&mutex);
    
    // Incrementa contatore se anomalia rilevata
    if (mse > AIConfig::ANOMALY_THRESHOLD) {
        anomalyCounter++;
        DEBUG_PRINT("Anomaly detected: MSE=%.5f, counter=%d", mse, anomalyCounter);
    } else {
        // Decrementa contatore se ritorna normale
        if (anomalyCounter > 0) {
            anomalyCounter--;
        }
        // Disattiva allarme se contatore raggiunge 0
        if (alarmActive && anomalyCounter == 0) {
            alarmActive = false;
            AI_DEBUG("✓ Alarm deactivated");
        }
    }

    // Attiva allarme quando raggiunge soglia
    if (anomalyCounter >= AIConfig::ALARM_TRIGGER_COUNT && !alarmActive) {
        alarmActive = true;
        AI_DEBUG("🚨 ANOMALY DETECTED! MSE: %.5f", mse);
    }

    currentMSE = mse;
    
    portEXIT_CRITICAL(&mutex);
}

void AIEngine::getSnapshot(float& out_mse, bool& out_alarm) {
    portENTER_CRITICAL(&mutex);
    out_mse = currentMSE;
    out_alarm = alarmActive;
    portEXIT_CRITICAL(&mutex);
}

float AIEngine::getMSE() const {
    return currentMSE;
}

bool AIEngine::hasAlarm() const {
    return alarmActive;
}

int AIEngine::getAnomalyCounter() const {
    return anomalyCounter;
}

void AIEngine::resetAlarm() {
    portENTER_CRITICAL(&mutex);
    alarmActive = false;
    anomalyCounter = 0;
    AI_DEBUG("Alarm manually reset");
    portEXIT_CRITICAL(&mutex);
}