#ifndef INC_HW_CALIBRATION_H_
#define INC_HW_CALIBRATION_H_

/* =========================================================
 * ====================== MOTORS ===========================
 * ========================================================= */

/* -------------------- PWM stop -------------------- */
#define PWM_STOP_FA            (3230u)
#define PWM_STOP_FB            (3230u)
#define PWM_STOP_BA            (3215u)
#define PWM_STOP_BB            (3215u)

/* -------------------- PWM scale forward -------------------- */
#define PWM_SCALE_FORWARD_FA   (690u)
#define PWM_SCALE_FORWARD_FB   (710u)
#define PWM_SCALE_FORWARD_BA   (690u)
#define PWM_SCALE_FORWARD_BB   (690u)

/* -------------------- PWM scale backward -------------------- */
#define PWM_SCALE_BACKWARD_FA  (640u)
#define PWM_SCALE_BACKWARD_FB  (630u)
#define PWM_SCALE_BACKWARD_BA  (630u)
#define PWM_SCALE_BACKWARD_BB  (620u)




/* =========================================================
 * ====================== ENCODERS =========================
 * ========================================================= */

/*
 * Encoder characteristics (common to all wheels)
 */
#define PULSES_PER_REVOLUTION  (24u)     /* impulsi encoder per giro motore */
#define GEAR_RATIO             (51.0f)   /* rapporto di riduzione motore -> ruota */

#endif /* INC_HW_CALIBRATION_H_ */
