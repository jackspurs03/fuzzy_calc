#ifndef NORMS_H
#define NORMS_H

// t-нормы
float t_min(float a, float b);
float t_prod(float a, float b);
float t_lukas(float a, float b);

// s-нормы
float s_max(float a, float b);
float s_asum(float a, float b);
float s_lukas(float a, float b);

#endif