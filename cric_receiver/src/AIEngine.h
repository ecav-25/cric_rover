// ============================================================================
// AIEngine.h
// ============================================================================

#ifndef AI_ENGINE_H
#define AI_ENGINE_H

#include "config.h"
#include "types.h"
#include "model.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"

class AIEngine {
private:
    uint8_t tensorArena[AIConfig::TENSOR_ARENA_SIZE];
    tflite::MicroErrorReporter errorReporter;
    tflite::AllOpsResolver resolver;
    const tflite::Model* model;
    tflite::MicroInterpreter* interpreter;
    TfLiteTensor* input;
    TfLiteTensor* output;
    
    float historyBuffer[AIConfig::TIME_STEPS][AIConfig::NUM_FEATURES];
    float inputBackup[AIConfig::TOTAL_INPUTS];

    float currentMSE;
    bool alarmActive;
    int anomalyCounter;
    unsigned long lastInferenceMs;
    unsigned long inferenceCount;
    
    portMUX_TYPE mutex = portMUX_INITIALIZER_UNLOCKED;

public:
    AIEngine();
    
    /**
     * Inizializza il modello TensorFlow Lite
     * @return true se inizializzazione riuscita
     */
    bool initialize();
    
    /**
     * Processa i dati telemetria per rilevamento anomalie
     * @param telemetry Dati telemetria rover
     */
    void processData(const RoverTelemetry& telemetry);
    
    /**
     * Legge snapshot thread-safe dello stato AI
     */
    void getSnapshot(float& out_mse, bool& out_alarm);
    
    /**
     * Getter per MSE attuale
     */
    float getMSE() const;
    
    /**
     * Getter per allarme
     */
    bool hasAlarm() const;
    
    /**
     * Getter per contatore anomalie
     */
    int getAnomalyCounter() const;
    
    /**
     * Reset manuale dello stato allarme
     */
    void resetAlarm();

private:
    /**
     * Aggiorna lo stato dell'allarme in base a MSE
     */
    void updateAlarmState(float mse);
    
    /**
     * Normalizza le features grezze in range [0, 1]
     */
    float normalizeFeature(float raw_value, int feature_idx);
};

#endif // AI_ENGINE_H