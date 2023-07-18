/**
 * @file
 * @brief Implementation of module TrackingInfo
 *
 * @copyright Copyright (c) 2020-2023 CERN, LUXE, TAU and the Corryvreckan authors.
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
    z_dut_ = config_.get<double>("z_dut");

    TDirectory* directory = getROOTDirectory();
    directory->cd();
    Tracks = new TTree("Tracks","Telescope tracking information in general");
    Tracks->Branch("triggerid", &triggerId);        Tracks->Branch("triggerids", &triggerId_vec);
    Tracks->Branch("timestamp", &triggerTimestamp); Tracks->Branch("timestamps", &triggerTimestamp_vec);
    Tracks->Branch("trackid", &trackid_vec)
    Tracks->Branch("x_dut", &x_dut); Tracks->Branch("y_dut", &y_dut);
    Tracks->Branch("chi2", &chi2_vec); Tracks->Branch("ndof", &ndof_vec);
    Tracks->Branch("num_cluster",  &num_cluster_vec);

    TTracks = new TTree("Tracks","Telescope tracking information in details");
    TTracks->Branch("triggerid", &triggerId);        TTracks->Branch("triggerids", &triggerId_vec);
    TTracks->Branch("timestamp", &triggerTimestamp); TTracks->Branch("timestamps", &triggerTimestamp_vec);
    TTracks->Branch("trackid", &trackid)
    TTracks->Branch("x_tel", &x_tel); TTracks->Branch("y_tel", &y_tel); TTracks->Branch("z_tel", &z_tel);
    TTracks->Branch("ax_tel", &ax_tel); TTracks->Branch("ay_tel", &ay_tel); TTracks->Branch("az_tel", &az_tel);
    TTracks->Branch("x_hit", &x_hit); TTracks->Branch("y_hit", &y_hit); TTracks->Branch("z_hit", &z_hit);
    TTracks->Branch("x_res", &x_res); TTracks->Branch("y_res", &y_res); TTracks->Branch("z_res", &z_res);
    TTracks->Branch("ux", &x_uncertainty); TTracks->Branch("uy", &y_uncertainty);
    TTracks->Branch("chi2", &chi2); TTracks->Branch("ndof", &ndof);
    TTracks->Branch("num_cluster",  &num_cluster); TTracks->Branch("cluster_plane", &cluster_status)

    // Initialise member variables
    m_eventNumber = 0;
}

StatusCode TrackingInfo::run(const std::shared_ptr<Clipboard>& clipboard) {

    // Check if this entry has any event
    if(!clipboard->isEventDefined()) {
        ModuleError("No Clipboard event defined, cannot continue");
    }

    // Loop over all tracks inside this event
    auto tracks = clipboard->getData<Track>();
    
    // Dumping Trigger ID and timestamp
    std::shared_ptr<Event> event = clipboard->getEvent();
    std::map<uint32_t, double> triggerIdTimestamp = event->triggerList();
    if (triggerIdTimestamp.size()!=1) {
        cout<<"Event "<<test.begin()->first<<" has more than one trigger!"<<endl;
    }
    triggerId_vec.clear(); triggerTimestamp_vec.clear();
    for (const auto& [id, timestamp] : triggerIdTimestamp) {
        triggerId = id;                 triggerId_vec.push_back(id);
        triggerTimestamp = timestamp;   triggerTimestamp_vec.push_back(timestamp);
    }

    // Clear vectors
    x_dut.clear(); y_dut.clear();
    x_tel.clear(); y_tel.clear(); z_tel.clear();
    ax_tel.clear(); ay_tel.clear(); az_tel.clear();
    x_res.clear(); y_res.clear(); z_res.clear();
    x_hit.clear(); y_hit.clear(); z_hit.clear();
    x_uncertainty.clear(); y_uncertainty.clear();
    chi2_vec.clear(); ndof_vec.clear();
    num_clusters_vec.clear();
    // Initialise vectors and tensor
    ROOT::Math::XYZPoint xyz_tel;
    ROOT::Math::XYZVector axyz_tel;
    ROOT::Math::XYZPoint xyz_hit;
    ROOT::Math::XYZPoint xyz_res;
    ROOT::Math::XYZPoint xyz_dut;
    TMatrixD uncertainty(3, 3);

    // Track-loop
    int track_id = 0;
    for (auto& track : tracks) {
        trackId = track_id;
        num_cluster = track->getNClusters();
        chi2 = track->getChi2(); ndof = track->getNdof();
        auto planes = track->getPlanes();
        uint num_plane = planes.size();
        auto plane = planes.begin();
        cluster_status = 0;
        for (uint i=0; i<num_plane; i++){
            if (plane->hasCluster()){
                cluster_status+= 1<<i;
                xyz_tel = track->getState(plane->getName());
                axyz_tel = track->getDirection(plane->getName());
                xyz_res = track->getGlobalResidual(plane->getName());
                xyz_hit = track->getClosestCluster(plane->getName())->global();
                uncertainty = track->getGlobalStateUncertainty(plane->getName());

                x_tel.push_back(xyz_tel.x()); y_tel.push_back(xyz_tel.y()); z_tel.push_back(xyz_tel.z());
                ax_tel.push_back(1.*axyz_tel.x()/axyz_tel.r());
                ay_tel.push_back(1.*axyz_tel.y()/axyz_tel.r());
                az_tel.push_back(1.*axyz_tel.z()/axyz_tel.r());
                x_res.push_back(xyz_res.x()); y_res.push_back(xyz_res.y()); z_res.push_back(xyz_res.z());
                x_hit.push_back(xyz_hit.x()); y_hit.push_back(xyz_hit.y()); z_hit.push_back(xyz_hit.z());
                x_uncertainty.push_back(uncertainty(0, 0));
                y_uncertainty.push_back(uncertainty(1, 1));
            }
            plane++;
        }
        TTrack.Fill();

        trackId_vec.push_back(trackId);
        xyz_dut = track->getIntercept(z_dut_);
        //xyz_dut = track->getInterceptTb22(z_dut_);
        x_dut.push_back(xyz_dut.x()); y_dut.push_back(xyz_dut.y());
        num_cluster_vec.push_back(num_cluster);
        chi2_vec.push_back(chi2); ndof_vec.push_back(ndof);

        track_id++;
    }
    
    Tracks->Fill();

    // Increment event counter
    m_eventNumber++;

    // Return value telling analysis to keep running
    return StatusCode::Success;
}

void TrackingInfo::finalize(const std::shared_ptr<ReadonlyClipboard>&) {

    LOG(DEBUG) << "Dumped " << m_eventNumber << " events";
}
