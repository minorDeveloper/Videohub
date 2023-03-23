#ifndef VIDEOHUB_VIDEOHUB_H
#define VIDEOHUB_VIDEOHUB_H

#include <string>
#include <vector>
#include <stdint.h>
#include <stdexcept>
#include <iterator>  // distance
#include <algorithm> // max_element

enum class DeviceStatus{
    connected,
    disconnected,
    needs_update
};

enum class LockStatus : std::uint8_t {
    owned,
    locked,
    unlocked,
};

enum class SerialDirection : std::uint8_t {
    control,
    slave,
    automatic,
};

enum class PortStatus : std::uint8_t {
    none,
    BNC,
    Optical,
    RS422,
};

class VideoHub {
    DeviceStatus status;
    std::string name;
    std::uint8_t videoInputs;
    std::uint8_t videoProcessingUnits;
    std::uint8_t videoOutputs;
    std::uint8_t videoMonitoringOutputs;
    std::uint8_t serialPorts;

    std::vector<std::string> inputLabels;
    std::vector<std::string> outputLabels;
    //std::vector<std::string> monitoringOutputLabels;
    //std::vector<std::string> serialPortLabels;
    //std::vector<std::string> frameLabels;

    std::vector<std::uint8_t> videoOutputRouting;
    //std::vector<std::uint8_t> monitoringOutputRouting;
    //std::vector<std::uint8_t> serialOutputRouting;
    //std::vector<std::uint8_t> processingUnitRouting;
    //std::vector<std::uint8_t> frameBufferRouting;

    std::vector<LockStatus> videoOutputLocks;
    //std::vector<LockStatus> monitoringOutputLocks;
    //std::vector<LockStatus> serialPortLocks;
    //std::vector<LockStatus> processingUnitLocks;
    //std::vector<LockStatus> frameBufferLocks;

    //std::vector<SerialDirection> serialPortDirections;

    //std::vector<PortStatus> videoInputStatus;
    //std::vector<PortStatus> videoOutputStatus;
    //std::vector<PortStatus> serialPortStatus;

    const std::string& longestString(const std::vector<std::string>& strings) const {
        auto it = std::max_element(strings.begin(), strings.end(),
                                   [](const auto& a, const auto&b) {return a.size() < b.size();});
        return *it;
        // Todo if the above doesn't work then use this
        // return strings.at(std::distance(strings.begin(), it));
    }

public:
    VideoHub(const DeviceStatus status, const std::string name,
             const std::uint8_t videoInputs            = 0,
             const std::uint8_t videoOutputs           = 0,
             const std::uint8_t videoProcessingUnits   = 0,
             const std::uint8_t videoMonitoringOutputs = 0,
             const std::uint8_t serialPorts            = 0) {

        this->status = status;
        this->name = name;
        this->videoInputs = videoInputs;
        this->videoProcessingUnits = videoProcessingUnits;
        this->videoOutputs = videoOutputs;
        this->videoMonitoringOutputs = videoMonitoringOutputs;
        this->serialPorts = serialPorts;

        this->inputLabels.resize(videoInputs, "");
        this->outputLabels.resize(videoOutputs, "");
        this->videoOutputRouting.resize(videoOutputs, 0);
        this->videoOutputLocks.resize(videoOutputs, LockStatus::unlocked);
    }

    const std::string& longestInputLabel() const {
        return longestString(this->inputLabels);
    }

    const std::string& longestOutputLabel() const {
        return longestString(this->outputLabels);
    }

    std::string getName() {
        return name;
    }

    std::string getSafeName() {
        std::string str = name;
        str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
        return str;
    }

    std::uint8_t inputs() {
        return videoInputs;
    }

    std::uint8_t outputs() {
        return videoOutputs;
    }


    //----------------------------------Input  labels----------------------------------
    void setVideoInputLabels(const std::vector<std::string> inputLabel) {
        if (inputLabel.size() != videoInputs) throw std::length_error("Array length different from number of video input ports");
        this->inputLabels = inputLabel;
    }

    void setVideoInputLabel(const std::uint8_t port, const std::string inputLabel) {
        if (port > videoInputs) throw std::length_error("Invalid port number");
        this->inputLabels.at(port) = inputLabel;
    }

    const std::vector<std::string>& getVideoInputLabels() const{
        return inputLabels;
    }

    const std::string& getVideoInputLabel(const std::uint8_t port) const{
        if (port > videoInputs) throw std::length_error("Invalid port number");
        return inputLabels.at(port);
    }

    //----------------------------------Output labels----------------------------------

    void setVideoOutputLabels(const std::vector<std::string> outputLabel) {
        if (outputLabel.size() != videoOutputs) throw std::length_error("Array length different from number of video output ports");
        this->outputLabels = outputLabel;
    }

    void setVideoOutputLabel(const std::uint8_t port, const std::string outputLabel) {
        if (port > videoOutputs) throw std::length_error("Invalid port number");
        this->outputLabels.at(port) = outputLabel;
    }

    const std::vector<std::string>& getVideoOutputLabels() const {
        return outputLabels;
    }

    const std::string& getVideoOutputLabel(const std::uint8_t port) const{
        if (port > videoOutputs) throw std::length_error("Invalid port number");
        return outputLabels.at(port);
    }


    //----------------------------------Output routing----------------------------------
    void setVideoOutputRouting(const std::vector<std::uint8_t> videoOutputRouting) {
        if (videoOutputRouting.size() != videoOutputs) throw std::length_error("Array length different from number of video output ports");
        for (std::uint8_t routing: videoOutputRouting) {
            if (routing > videoInputs) throw std::length_error("Invalid routing");
        }
        this->videoOutputRouting = videoOutputRouting;
    }

    void setVideoOutputRouting(const std::uint8_t port, const std::uint8_t routing) {
        if (port > videoOutputs) throw std::length_error("Invalid port number");
        if (routing > videoInputs) throw std::length_error("Invalid routing");
        this->videoOutputRouting.at(port) = routing;
    }

    const std::vector<std::uint8_t>& getVideoOutputRouting() const {
        return videoOutputRouting;
    }

    const std::uint8_t& getVideoOutputRouting(const std::uint8_t port) const{
        if (port > videoOutputs) throw std::length_error("Invalid port number");
        return videoOutputRouting.at(port);
    }

    //----------------------------------Output locks----------------------------------
    void setVideoOutputLocks(const std::vector<LockStatus> videoOutputLocks) {
        if (videoOutputLocks.size() != videoOutputs) throw std::length_error("Array length different from number of video output ports");
        this->videoOutputLocks = videoOutputLocks;
    }

    void setVideoOutputLocks(const std::uint8_t port, const LockStatus lock) {
        if (port > videoOutputs) throw std::length_error("Invalid port number");
        this->videoOutputLocks.at(port) = lock;
    }

    const std::vector<LockStatus>& getVideoOutputLocks() const{
        return videoOutputLocks;
    }

    const LockStatus& getVideoOutputLock(const std::uint8_t port) const{
        if (port > videoOutputs) throw std::length_error("Invalid port number");
        return videoOutputLocks.at(port);
    }
};


#endif //VIDEOHUB_VIDEOHUB_H
