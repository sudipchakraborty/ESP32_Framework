#include "AppTime.h"
//_____________________________________________________________________________________________________________
/**
 * @brief Constructs a new Time object and initializes internal variables.
 *
 * This constructor sets the initial state of the timer by resetting
 * the start time to 0, timeout value to 0, and marking the timer as stopped.
 *
 * @note The timer does not start automatically upon construction.
 */
AppTime::AppTime() {
    startTime = 0;
    timeoutValue = 0;
    timerRunning = false;
}

//_____________________________________________________________________________________________________________
/**
 * @brief Resets the timer to its initial state.
 *
 * This function stops the timer (if running) and clears the start time.
 * The timeout value remains unchanged, so you can restart the timer
 * with the same timeout period using @ref time_start().
 *
 * @note After calling this function, @ref timeOut() will always return false
 * until @ref time_start() is called again.
 */
void AppTime::reset() {
    startTime = 0;
    timerRunning = false;
}

//_____________________________________________________________________________________________________________
/**
 * @brief Starts the timer if it is not already running.
 *
 * This function records the current system time using `millis()` as the start time
 * and marks the timer as active. If the timer is already running, this function
 * does nothing to prevent unintentional resets.
 *
 * @note To restart the timer regardless of its state, call @ref time_reset() first.
 */
void AppTime::start() {
    if (!timerRunning) {
        startTime = millis();
        timerRunning = true;
    }
}

//_____________________________________________________________________________________________________________
/**
 * @brief Sets the timeout duration for the timer.
 *
 * Defines the interval (in milliseconds) after which @ref timeOut() will
 * return `true` once the timer has been started.
 *
 * @param timeout Timeout duration in milliseconds.
 *
 * @note The timeout value remains active until explicitly changed by another call.
 */
void AppTime::set_time(unsigned long timeout) {
    timeoutValue = timeout;
}

//_____________________________________________________________________________________________________________
/**
 * @brief Checks whether the configured timeout period has elapsed.
 *
 * This function compares the elapsed time since @ref time_start() with
 * the configured timeout value set using @ref set_time().
 *
 * @return `true` if the timeout period has elapsed, otherwise `false`.
 *
 * @note Once the timeout occurs, the timer automatically stops.
 * To restart, call @ref time_start() again.
 */
bool AppTime::timeOut() {
    if (timerRunning && (millis() - startTime >= timeoutValue)) {
        timerRunning = false;
        return true;
    }
    return false;
}

//_____________________________________________________________________________________________________________
/**
 * @brief Returns the elapsed time in milliseconds since the timer started.
 *
 * Calculates the time difference between the current system time (`millis()`)
 * and the recorded start time.
 *
 * @return Elapsed time in milliseconds.
 *
 * @note If the timer is not currently running, this function returns 0.
 * To get the elapsed duration even after timeout, modify this behavior
 * to store the last elapsed value before stopping the timer.
 */
unsigned long AppTime::elapsed_time() {
    if (timerRunning) {
        return millis() - startTime;
    } else {
        return 0;
    }
}
//_____________________________________________________________________________________________________________
void AppTime::set_on_time(unsigned long onTimeValue) {
    onTime = onTimeValue;
}
//_____________________________________________________________________________________________________________
void AppTime::set_off_time(unsigned long offTimeValue) {
    offTime = offTimeValue;
}
//_____________________________________________________________________________________________________________
bool AppTime::on_time_expired(unsigned long curentTime) {
    if (curentTime >= onTime) {
        return true;
    }
    return false;
}
//_____________________________________________________________________________________________________________
bool AppTime::off_time_expired(unsigned long curentTime) {
    if (curentTime >= offTime) {
        return true;
    }
    return false;
}
//_____________________________________________________________________________________________________________
bool AppTime::isWithinWindow(unsigned long currentTimeSec)
{
    // Case 1: Normal window (same day)
    if (onTime < offTime)
    {
        return (currentTimeSec >= onTime && currentTimeSec < offTime);
    }
    // Case 2: Cross-midnight (e.g. 22:00 → 06:00)
    else
    {
        return (currentTimeSec >= onTime || currentTimeSec < offTime);
    }
}