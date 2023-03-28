#include "platform_config.h"
#include "os_3ds.h"

void OS_3DS::initialize() {

}

void OS_3DS::initialize_joypads() {

}

void OS_3DS::set_main_loop(MainLoop *p_main_loop){
    
}

MainLoop *OS_3DS::get_main_loop() const {
    return nullptr;
}

void OS_3DS::delete_main_loop() {

}

void OS_3DS::finalize() {

}

void OS_3DS::finalize_core() {

}

bool OS_3DS::_check_internal_feature_support(const String &p_feature) {
    return false;
}

Vector<String> OS_3DS::get_video_adapter_driver_info() const {
    return Vector<String>();
}

String OS_3DS::get_stdin_string() {
    return String();
}

Error OS_3DS::get_entropy(uint8_t *r_buffer, int p_bytes) {
    return Error::ERR_UNAVAILABLE;
}

Error OS_3DS::execute(const String &p_path, const List<String> &p_arguments, String *r_pipe, int *r_exitcode, bool read_stderr, Mutex *p_pipe_mutex, bool p_open_console)
{
    return Error::ERR_UNAVAILABLE;
}

Error OS_3DS::create_process(const String &p_path, const List<String> &p_arguments, ProcessID *r_child_id, bool p_open_console)
{
    return Error::ERR_UNAVAILABLE;
}

Error OS_3DS::kill(const ProcessID &p_pid)
{
    return Error::ERR_UNAVAILABLE;
}

bool OS_3DS::is_process_running(const ProcessID &p_pid) const {
    return false;
}

bool OS_3DS::has_environment(const String &p_var) const {
    return false;
}

String OS_3DS::get_environment(const String &p_var) const {
    return String();
}

void OS_3DS::set_environment(const String &p_var, const String &p_value) const {

}

void OS_3DS::unset_environment(const String &p_var) const {

}

String OS_3DS::get_name() const {
    return "Nintendo 3DS";
}

String OS_3DS::get_distribution_name() const {
    return "Horizon";
}

String OS_3DS::get_version() const {
    return String();
}

OS::DateTime OS_3DS::get_datetime(bool utc) const {
    return OS::DateTime();
}

OS::TimeZoneInfo OS_3DS::get_time_zone_info() const {
    return OS::TimeZoneInfo();
}

void OS_3DS::delay_usec(uint32_t p_usec) const {

}

uint64_t OS_3DS::get_ticks_usec() const{
    return 0;
}