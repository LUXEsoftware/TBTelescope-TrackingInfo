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
//     config_.setDefault<double>("z_ref0", 0.0);
//     config_.setDefault<double>("z_ref1", 0.0);
//     config_.setDefault<double>("z_ref2", 0.0);
//     config_.setDefault<double>("z_ref3", 0.0);
//     config_.setDefault<double>("z_ref4", 0.0);
//     config_.setDefault<double>("z_ref5", 0.0);

//     z_ref0_ = config_.get<double>("z_ref0");
//     z_ref1_ = config_.get<double>("z_ref1");
//     z_ref2_ = config_.get<double>("z_ref2");
//     z_ref3_ = config_.get<double>("z_ref3");
//     z_ref4_ = config_.get<double>("z_ref4");
//     z_ref5_ = config_.get<double>("z_ref5");
    z_dut_ = config_.get<double>("z_dut");

    TDirectory* directory = getROOTDirectory();
    directory->cd();
    Tracks = new TTree("Tracks","Telescope tracking information");
    Tracks->Branch("triggerid", &triggerId);
    Tracks->Branch("timestamp", &triggerTimestamp);
    Tracks->Branch("x_dut", &x_dut);
    Tracks->Branch("y_dut", &y_dut);
    Tracks->Branch("chi2", &chi2);
    Tracks->Branch("ndof", &ndof);
    Tracks->Branch("num_clusters",  &num_clusters);
    // Tracks->Branch("num_planes",  &num_planes);
    Tracks->Branch("x_tel0", &x_tel[0]);
    Tracks->Branch("y_tel0", &y_tel[0]);
    Tracks->Branch("z_tel0", &z_tel[0]);
    Tracks->Branch("x_tel1", &x_tel[1]);
    Tracks->Branch("y_tel1", &y_tel[1]);
    Tracks->Branch("z_tel1", &z_tel[1]);
    Tracks->Branch("x_tel2", &x_tel[2]);
    Tracks->Branch("y_tel2", &y_tel[2]);
    Tracks->Branch("z_tel2", &z_tel[2]);
    Tracks->Branch("x_tel3", &x_tel[3]);
    Tracks->Branch("y_tel3", &y_tel[3]);
    Tracks->Branch("z_tel3", &z_tel[3]);
    Tracks->Branch("x_tel4", &x_tel[4]);
    Tracks->Branch("y_tel4", &y_tel[4]);
    Tracks->Branch("z_tel4", &z_tel[4]);
    Tracks->Branch("x_tel5", &x_tel[5]);
    Tracks->Branch("y_tel5", &y_tel[5]);
    Tracks->Branch("z_tel5", &z_tel[5]);
    Tracks->Branch("x_res0", &x_res[0]);
    Tracks->Branch("y_res0", &y_res[0]);
    Tracks->Branch("x_res1", &x_res[1]);
    Tracks->Branch("y_res1", &y_res[1]);
    Tracks->Branch("x_res2", &x_res[2]);
    Tracks->Branch("y_res2", &y_res[2]);
    Tracks->Branch("x_res3", &x_res[3]);
    Tracks->Branch("y_res3", &y_res[3]);
    Tracks->Branch("x_res4", &x_res[4]);
    Tracks->Branch("y_res4", &y_res[4]);
    Tracks->Branch("x_res5", &x_res[5]);
    Tracks->Branch("y_res5", &y_res[5]);
    Tracks->Branch("ux_0", &x_uncertainty[0]);
    Tracks->Branch("uy_0", &y_uncertainty[0]);
    Tracks->Branch("ux_1", &x_uncertainty[1]);
    Tracks->Branch("uy_1", &y_uncertainty[1]);
    Tracks->Branch("ux_2", &x_uncertainty[2]);
    Tracks->Branch("uy_2", &y_uncertainty[2]);
    Tracks->Branch("ux_3", &x_uncertainty[3]);
    Tracks->Branch("uy_3", &y_uncertainty[3]);
    Tracks->Branch("ux_4", &x_uncertainty[4]);
    Tracks->Branch("uy_4", &y_uncertainty[4]);
    Tracks->Branch("ux_5", &x_uncertainty[5]);
    Tracks->Branch("uy_5", &y_uncertainty[5]);

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
    for (uint i=0; i<6; i++){
        x_tel[i].clear(); y_tel[i].clear(); z_tel[i].clear();
        x_res[i].clear(); y_res[i].clear();
        x_uncertainty[i].clear(); y_uncertainty[i].clear();
    }
    x_dut.clear(); y_dut.clear();
    chi2.clear(); ndof.clear();
    num_clusters.clear(); // num_planes.clear();
    ROOT::Math::XYPoint xy_res[6];
    ROOT::Math::XYZPoint xyz_tel[6];
    ROOT::Math::XYZPoint xyz_dut;
    TMatrixD uncertainty(3, 3);
    for (auto& track : tracks) {
        uint num_cluster = track->getNClusters();
        auto planes = track->getPlanes();
        auto plane = planes.begin();
        uint num_plane = planes.size();
        // num_planes.push_back(num_plane);
        for (uint i=0; i<num_plane; i++){
            if (plane->hasCluster()){
                xy_res[i] = track->getLocalResidual(plane->getName());
                xyz_tel[i] = track->getState(plane->getName());
                uncertainty = track->getGlobalStateUncertainty(plane->getName());
                x_uncertainty[i].push_back(uncertainty(0, 0));
                y_uncertainty[i].push_back(uncertainty(1, 1));

                x_res[i].push_back(xy_res[i].x()); y_res[i].push_back(xy_res[i].y());
                x_tel[i].push_back(xyz_tel[i].x()); y_tel[i].push_back(xyz_tel[i].y());
                z_tel[i].push_back(xyz_tel[i].z());
            }
            plane++;
        }
        xyz_dut = track->getIntercept(z_dut_);
        //xyz_dut = track->getInterceptTb22(z_dut_);
        x_dut.push_back(xyz_dut.x()); y_dut.push_back(xyz_dut.y());
        chi2.push_back(track->getChi2()); ndof.push_back(track->getNdof());
        num_clusters.push_back(num_cluster);
    }
    
    triggerId.clear();
    triggerTimestamp.clear();
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
