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
    config_.setDefault<double>("z_ref0", 0.0);
    config_.setDefault<double>("z_ref1", 0.0);
    config_.setDefault<double>("z_ref2", 0.0);
    config_.setDefault<double>("z_ref3", 0.0);
    config_.setDefault<double>("z_ref4", 0.0);
    config_.setDefault<double>("z_ref5", 0.0);

    z_ref0_ = config_.get<double>("z_ref0");
    z_ref1_ = config_.get<double>("z_ref1");
    z_ref2_ = config_.get<double>("z_ref2");
    z_ref3_ = config_.get<double>("z_ref3");
    z_ref4_ = config_.get<double>("z_ref4");
    z_ref5_ = config_.get<double>("z_ref5");
    z_dut_ = config_.get<double>("z_dut");

    TDirectory* directory = getROOTDirectory();
    directory->cd();
    Tracks = new TTree("Tracks","Telescope tracking information");
    Tracks->Branch("triggerid", &triggerId);
    Tracks->Branch("timestamp", &triggerTimestamp);
    Tracks->Branch("x_dut", &x_dut);
    Tracks->Branch("y_dut", &y_dut);
    Tracks->Branch("x_tel5", &x_tel5);
    Tracks->Branch("y_tel5", &y_tel5);
    Tracks->Branch("x_tel0", &x_tel0);
    Tracks->Branch("y_tel0", &y_tel0);
    Tracks->Branch("x_tel1", &x_tel1);
    Tracks->Branch("y_tel1", &y_tel1);
    Tracks->Branch("x_tel2", &x_tel2);
    Tracks->Branch("y_tel2", &y_tel2);
    Tracks->Branch("x_tel3", &x_tel3);
    Tracks->Branch("y_tel3", &y_tel3);
    Tracks->Branch("x_tel4", &x_tel4);
    Tracks->Branch("y_tel4", &y_tel4);
    Tracks->Branch("x_res5", &x_res5);
    Tracks->Branch("y_res5", &y_res5);
    Tracks->Branch("x_res0", &x_res0);
    Tracks->Branch("y_res0", &y_res0);
    Tracks->Branch("x_res1", &x_res1);
    Tracks->Branch("y_res1", &y_res1);
    Tracks->Branch("x_res2", &x_res2);
    Tracks->Branch("y_res2", &y_res2);
    Tracks->Branch("x_res3", &x_res3);
    Tracks->Branch("y_res3", &y_res3);
    Tracks->Branch("x_res4", &x_res4);
    Tracks->Branch("y_res4", &y_res4);

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

    auto tracks = clipboard->getData<GblTrack>();
//    // Skip event with 0 or more than 1 tracks
//    if(tracks.size() != 1) {
//        return StatusCode::Success;
//    }
//    if(!tracks.size()) {
//        std::cout <<"no track in this event"<<std::endl;
//    }
    // Loop over all tracks inside this event
    for (auto& track : tracks) {
        ROOT::Math::XYPoint xy_res0 = track->getLocalResidual("ADENIUM_0");
        ROOT::Math::XYPoint xy_res1 = track->getLocalResidual("ADENIUM_1");
        ROOT::Math::XYPoint xy_res2 = track->getLocalResidual("ADENIUM_2");
        ROOT::Math::XYPoint xy_res3 = track->getLocalResidual("ADENIUM_3");
        ROOT::Math::XYPoint xy_res4 = track->getLocalResidual("ADENIUM_4");
        ROOT::Math::XYPoint xy_res5 = track->getLocalResidual("ADENIUM_5");
        ROOT::Math::XYZPoint xyz_tel0 = track->getIntercept(z_ref0_);
        ROOT::Math::XYZPoint xyz_tel1 = track->getIntercept(z_ref1_);
        ROOT::Math::XYZPoint xyz_tel2 = track->getIntercept(z_ref2_);
        ROOT::Math::XYZPoint xyz_tel3 = track->getIntercept(z_ref3_);
        ROOT::Math::XYZPoint xyz_tel4 = track->getIntercept(z_ref4_);
        ROOT::Math::XYZPoint xyz_tel5 = track->getIntercept(z_ref5_);
        ROOT::Math::XYZPoint xyz_dut = track->getIntercept(z_dut_);
        x_res0.push_back(xy_res0.x()); y_res0.push_back(xy_res0.y());
        x_res1.push_back(xy_res1.x()); y_res1.push_back(xy_res1.y());
        x_res2.push_back(xy_res2.x()); y_res2.push_back(xy_res2.y());
        x_res3.push_back(xy_res3.x()); y_res3.push_back(xy_res3.y());
        x_res4.push_back(xy_res4.x()); y_res4.push_back(xy_res4.y());
        x_res5.push_back(xy_res5.x()); y_res5.push_back(xy_res5.y());
        x_tel0.push_back(xyz_tel0.x()); y_tel0.push_back(xyz_tel0.y());
        x_tel1.push_back(xyz_tel1.x()); y_tel1.push_back(xyz_tel1.y());
        x_tel2.push_back(xyz_tel2.x()); y_tel2.push_back(xyz_tel2.y());
        x_tel3.push_back(xyz_tel3.x()); y_tel3.push_back(xyz_tel3.y());
        x_tel4.push_back(xyz_tel4.x()); y_tel4.push_back(xyz_tel4.y());
        x_tel5.push_back(xyz_tel5.x()); y_tel5.push_back(xyz_tel5.y());
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
