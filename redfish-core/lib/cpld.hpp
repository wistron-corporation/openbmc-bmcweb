#pragma once

#include <app.hpp>
#include <registries/privilege_registry.hpp>
#include <utils/fw_utils.hpp>
namespace redfish
{
/**
 * CpldService class supports handle get method for cpld.
 */
inline void
    handleCpldServiceGet(const crow::Request&,
                         const std::shared_ptr<bmcweb::AsyncResp>& asyncResp)
{
    asyncResp->res.jsonValue["@odata.id"] = "/redfish/v1/Systems/system/Cpld";
    asyncResp->res.jsonValue["@odata.type"] = "#Cpld.v1_0_0.Cpld";
    asyncResp->res.jsonValue["Name"] = "CPLD Configuration";
    asyncResp->res.jsonValue["Description"] = "CPLD Configuration Service";
    asyncResp->res.jsonValue["Id"] = "CPLD";

    // Get the ActiveSoftwareImage and SoftwareImages
    fw_util::populateFirmwareInformation(asyncResp, fw_util::cpldPurpose, "",
                                         true);
}
inline void requestRoutesCpldService(App& app)
{
    BMCWEB_ROUTE(app, "/redfish/v1/Systems/system/Cpld/")
        .privileges(redfish::privileges::getCpld)
        .methods(boost::beast::http::verb::get)(handleCpldServiceGet);
}

/**
 * CpldReset class supports handle POST method for Reset cpld.
 * The class retrieves and sends data directly to D-Bus.
 *
 * Function handles POST method request.
 * Analyzes POST body message before sends Reset request data to D-Bus.
 */
inline void
    handleCpldResetPost(const crow::Request&,
                        const std::shared_ptr<bmcweb::AsyncResp>& asyncResp)
{
    crow::connections::systemBus->async_method_call(
        [asyncResp](const boost::system::error_code ec) {
            if (ec)
            {
                BMCWEB_LOG_ERROR << "Failed to reset cpld: " << ec;
                messages::internalError(asyncResp->res);
                return;
            }
        },
        "org.open_power.Software.CPLD.Updater", "/xyz/openbmc_project/software",
        "xyz.openbmc_project.Common.FactoryReset", "Reset");
}

} // namespace redfish
