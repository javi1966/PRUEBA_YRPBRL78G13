#ifndef __RTC_H
#define __RTC_H

#define RTC_WAITTIME                320U

struct RTCCounterValue
{
    unsigned char Sec;
    unsigned char Min;
    unsigned char Hour;
    unsigned char Day;
    unsigned char Week;
    unsigned char Month;
    unsigned char Year;
} ;
struct RTCAlarmValue
{
    unsigned char Alarmwm;
    unsigned char Alarmwh;
    unsigned char Alarmww;
};
enum RTCHourSystem
{
    HOUR12, HOUR24
};
enum RTCINTPeriod
{
    HALFSEC = 1U, ONESEC, ONEMIN, ONEHOUR, ONEDAY, ONEMONTH
};
enum RTCINTInterval
{
    INTERVAL0, INTERVAL1, INTERVAL2, INTERVAL3, INTERVAL4, INTERVAL5, INTERVAL6
};

//***************************************************************************************


void initRTC();
void ponHora(const unsigned char hora,const unsigned char min);
unsigned char bin2bcd(unsigned char bin);
unsigned char bcd2bin(unsigned char BCD);


#endif 