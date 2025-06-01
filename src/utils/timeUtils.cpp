#include "utils/timeUtils.h"

namespace Utils {
    // Calculate AQI for PM2.5 based on EPA standards
    float calculatePM25AQI(float pm25) {
        // EPA breakpoints for PM2.5 in μg/m³
        if (pm25 <= 12.0) {
            return map(pm25, 0, 12.0, 0, 50);
        } else if (pm25 <= 35.4) {
            return map(pm25, 12.1, 35.4, 51, 100);
        } else if (pm25 <= 55.4) {
            return map(pm25, 35.5, 55.4, 101, 150);
        } else if (pm25 <= 150.4) {
            return map(pm25, 55.5, 150.4, 151, 200);
        } else if (pm25 <= 250.4) {
            return map(pm25, 150.5, 250.4, 201, 300);
        } else if (pm25 <= 350.4) {
            return map(pm25, 250.5, 350.4, 301, 400);
        } else {
            return map(pm25, 350.5, 500.4, 401, 500);
        }
    }
    
    // Calculate AQI for PM10 based on EPA standards
    float calculatePM10AQI(float pm10) {
        // EPA breakpoints for PM10 in μg/m³
        if (pm10 <= 54) {
            return map(pm10, 0, 54, 0, 50);
        } else if (pm10 <= 154) {
            return map(pm10, 55, 154, 51, 100);
        } else if (pm10 <= 254) {
            return map(pm10, 155, 254, 101, 150);
        } else if (pm10 <= 354) {
            return map(pm10, 255, 354, 151, 200);
        } else if (pm10 <= 424) {
            return map(pm10, 355, 424, 201, 300);
        } else if (pm10 <= 504) {
            return map(pm10, 425, 504, 301, 400);
        } else {
            return map(pm10, 505, 604, 401, 500);
        }
    }
    
    // Calculate overall AQI (worst of PM2.5 and PM10)
    float calculateOverallAQI(float pm25, float pm10) {
        float pm25AQI = calculatePM25AQI(pm25);
        float pm10AQI = calculatePM10AQI(pm10);
        
        return max(pm25AQI, pm10AQI);
    }
}
