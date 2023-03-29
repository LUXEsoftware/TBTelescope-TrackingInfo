/**
 * @file
 * @brief Definition of module TrackingInfo
 *
 * @copyright Copyright (c) 2020 CERN and the Corryvreckan authors.
 * This software is distributed under the terms of the MIT License, copied verbatim in the file "LICENSE.md".
 * In applying this license, CERN does not waive the privileges and immunities granted to it by virtue of its status as an
 * Intergovernmental Organization or submit itself to any jurisdiction.
 */

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
    /** @ingroup Modules
     * @brief Module to do function
     *
     * More detailed explanation of module
     */
    class TrackingInfo : public Module {

    public:
        /**
         * @brief Constructor for this unique module
         * @param config Configuration object for this module as retrieved from the steering file
         * @param detectors Vector of pointers to the detectors
         */
        TrackingInfo(Configuration& config, std::vector<std::shared_ptr<Detector>> detectors);

        /**
         * @brief [Initialise this module]
         */
        void initialize() override;

        /**
         * @brief [Run the function of this module]
         */
        StatusCode run(const std::shared_ptr<Clipboard>& clipboard) override;

        /**
         * @brief [Finalise module]
         */
        void finalize(const std::shared_ptr<ReadonlyClipboard>& clipboard) override;

    private:
        int m_eventNumber;
        double z_dut_;
//         double z_ref0_;
//         double z_ref1_;
//         double z_ref2_;
//         double z_ref3_;
//         double z_ref4_;
//         double z_ref5_;
        TTree* Tracks;
        std::vector<double> x_tel[6], y_tel[6];
        std::vector<double> x_dut, y_dut;
        std::vector<double> x_res[6], y_res[6], x_uncertainty[6], y_uncertainty[6];
        std::vector<uint32_t> triggerId;
        std::vector<double> triggerTimestamp;
        std::vector<double> chi2;
        std::vector<int> ndof;

    };

} // namespace corryvreckan
