/**
 * @file
 * @brief Definition of module TrackingInfo
 *
 * @copyright Copyright (c) 2020-2023 CERN, LUXE, TAU and the Corryvreckan authors.
 * This software is distributed under the terms of the MIT License, copied verbatim in the file "LICENSE.md".
 * In applying this license, CERN does not waive the privileges and immunities granted to it by virtue of its status as an
 * Intergovernmental Organization or submit itself to any jurisdiction.
 */

#ifndef TrackingInfo_H
#define TrackingInfo_H

#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <iostream>
#include "core/module/Module.hpp"
#include "objects/Cluster.hpp"
#include "objects/Pixel.hpp"
#include "objects/Track.hpp"

namespace corryvreckan {
    /** @ingroup TrackingInfo
     * @brief Dump the details of the telescope tracks
     *
     * Go over through track on the clipboard and extra information and store on TTree object(s).
     */
    class TrackingInfo : public Module {

    public:
        /**
         * @brief Constructor for TrackingInfo
         * @param config Configuration object for this module as retrieved from the steering file
         * @param detectors Vector of pointers to the detectors
         */
        TrackingInfo(Configuration& config, std::vector<std::shared_ptr<Detector>> detectors);

        /**
         * @brief [Initialise TrackingInfo]
         */
        void initialize() override;

        /**
         * @brief [Run the function of TrackingInfo]
         */
        StatusCode run(const std::shared_ptr<Clipboard>& clipboard) override;

        /**
         * @brief [Finalise TrackingInfo]
         */
        void finalize(const std::shared_ptr<ReadonlyClipboard>& clipboard) override;

    private:
        // Event counter
        int m_eventNumber;
        // Common information
        uint32_t triggerId;         std::vector<uint32_t> triggerId_vec;
        double triggerTimestamp;    std::vector<double> triggerTimestamp_vec;
        // Storage TTree objects
        TTree* Tracks;
        std::vector<ushort> trackId_vec;
        std::vector<double> x_dut, y_dut;
        std::vector<double> chi2_vec; std::vector<uint> ndof_vec;
        std::vector<uint> num_cluster_vec;
        // Storage TTree objects
        TTree* TTracks;
        ushort trackId;
        std::vector<double> x_tel, y_tel, z_tel;
        std::vector<double> ax_tel, ay_tel, az_tel;
        std::vector<double> x_res, y_res, z_res;
        std::vector<double> x_hit, y_hit, z_hit;
        std::vector<double> x_uncertainty, y_uncertainty;
        double chi2; uint ndof;
        ushort num_cluster;
        ushort cluster_status; // Bitwise cluster status code (e.g. 111011)

        // Parameters
        double z_dut_;
    };

} // namespace corryvreckan


#endif // TrackingInfo_H