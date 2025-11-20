#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

inline std::string layNgayDangKy(time_t ngayDK) {
    tm* timeinfo = localtime(&ngayDK);
    char buffer[15];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeinfo);
    return std::string(buffer);
}

// Hàm chuyển đổi time_t sang chuỗi ngày
inline string timeString(time_t t) {
    tm* ltm = localtime(&t);
    char buffer[20];
    sprintf(buffer, "%02d/%02d/%04d",
            ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
    return string(buffer);
}

inline string layThoiGianHeThong() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[30];
    sprintf(buffer, "%02d/%02d/%04d %02d:%02d:%02d",
            ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900,
            ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return string(buffer);
}


#endif
