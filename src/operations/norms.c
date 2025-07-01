#include "norms.h"
#include <math.h>

// Вспомогательная функция для ограничения значений
static inline float clamp(float value) {
    return fmaxf(0.0f, fminf(1.0f, value));
}

// t-нормы
float t_min(float a, float b) {
    a = clamp(a);
    b = clamp(b);
    return (a < b) ? a : b;
}

float t_prod(float a, float b) {
    a = clamp(a);
    b = clamp(b);
    return a * b;
}

float t_lukas(float a, float b) {
    a = clamp(a);
    b = clamp(b);
    return fmaxf(0.0f, a + b - 1.0f);
}

// s-нормы
float s_max(float a, float b) {
    a = clamp(a);
    b = clamp(b);
    return (a > b) ? a : b;
}

float s_asum(float a, float b) {
    a = clamp(a);
    b = clamp(b);
    return a + b - a * b;
}

float s_lukas(float a, float b) {
    a = clamp(a);
    b = clamp(b);
    return fminf(1.0f, a + b);
}