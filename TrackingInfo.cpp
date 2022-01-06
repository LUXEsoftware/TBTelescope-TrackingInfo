/**
 * @file
 * @brief Implementation of module TrackingInfo
 *
 * @copyright Copyright (c) 2020 CERN and the Corryvreckan authors.
 * This software is distributed under the terms of the MIT License, copied verbatim in the file "LICENSE.md".
 * In applying this license, CERN does not waive the privileges and immunities granted to it by virtue of its status as an
 * Intergovernmental Organization or submit itself to any jurisdiction.
 */

#include "TrackingInfo.h"
#include "objects/Event.hpp"

using namespace corryvreckan;

TrackingInfo::TrackingInfo(Configuration& config, std::vector<std::shared_ptr<Detector>> detectors)
    : Module(config, std::move(detectors)) {}

void TrackingInfo::initialize() {

    // Read value from .conf file
    config_.setDefault<double>("set_z_ref", 1000.);

    z_ref  = config_.get<double>("set_z_ref");
    z_dut = config_.get<double>("set_z_dut");

    TDirectory* directory = getROOTDirectory();
    directory->cd();
    Tracks = new TTree("Tracks","Tracking information");
    Tracks->Branch("x_ref", &x_ref);
    Tracks->Branch("y_ref", &y_ref);
    Tracks->Branch("x_dut", &x_dut);
    Tracks->Branch("y_dut", &y_dut);
    Tracks->Branch("triggerid", &triggerId);
    Tracks->Branch("timestamp", &triggerTimestamp);

//    for(auto& detector : get_detectors()) {
//        LOG(DEBUG) << "Initialise for detector " + detector->getName();
//    }

    // Initialise member variables
    m_eventNumber = 0;
}

StatusCode TrackingInfo::run(const std::shared_ptr<Clipboard>& clipboard) {

    // Check if this entry has any event
    if(!clipboard->isEventDefined()) {
        ModuleError("No Clipboard event defined, cannot continue");
    }

    auto tracks = clipboard->getData<Track>();
//    // Skip event with 0 or more than 1 tracks
//    if(tracks.size() != 1) {
//        return StatusCode::Success;
//    }
//    if(!tracks.size()) {
//        std::cout <<"no track in this event"<<std::endl;
//    }
    // Loop over all tracks inside this event
    x_ref = {};
    y_ref = {};
    x_dut = {};
    y_dut = {};
    for (auto& track : tracks) {
        ROOT::Math::XYZPoint xyz_ref = track->getIntercept(z_ref);
        ROOT::Math::XYZPoint xyz_dut = track->getIntercept(z_dut);
        x_ref.push_back(xyz_ref.x()); y_ref.push_back(xyz_ref.y());
        x_dut.push_back(xyz_dut.x()); y_dut.push_back(xyz_dut.y());
    }
    
    triggerId = {};
    triggerTimestamp = {};
    // Dumping Trigger ID and timestamp
    std::shared_ptr<Event> event = clipboard->getEvent();
    std::map<uint32_t, double> triggerIdTimestamp = event->triggerList();
    for (const auto& [id, timestamp] : triggerIdTimestamp) {
        triggerId.push_back(id);
        triggerTimestamp.push_back(timestamp);
    }
    
    Tracks->Fill();

    // Increment event counter
    m_eventNumber++;

    // Return value telling analysis to keep running
    return StatusCode::Success;
}

void TrackingInfo::finalize(const std::shared_ptr<ReadonlyClipboard>&) {

    LOG(DEBUG) << "Analysed " << m_eventNumber << " events";
}
