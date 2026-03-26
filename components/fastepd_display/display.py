import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.const import CONF_ID

AUTO_LOAD = ["display"]

fastepd_display_ns = cg.esphome_ns.namespace("fastepd_display")
FastEPDDisplay = fastepd_display_ns.class_(
    "FastEPDDisplay", cg.PollingComponent, display.DisplayBuffer
)

CONF_FULL_UPDATE_CLEAR_MODE = "full_update_clear_mode"

FASTEPD_GIT_REF = "1.4.6"
FASTEPD_REPOSITORY = f"https://github.com/bitbank2/FastEPD.git#{FASTEPD_GIT_REF}"

CLEAR_MODES = {
    "none": 0,
    "fast": 1,
    "slow": 2,
    "white": 3,
    "extra_white": 4,
    "black": 5,
}

CONFIG_SCHEMA = cv.All(
    display.FULL_DISPLAY_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(FastEPDDisplay),
            cv.Optional(CONF_FULL_UPDATE_CLEAR_MODE, default="slow"): cv.enum(
                CLEAR_MODES, lower=True
            ),
        }
    ).extend(cv.polling_component_schema("never")),
    cv.only_on_esp32,
    cv.only_with_arduino,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await display.register_display(var, config)

    cg.add(var.set_full_update_clear_mode(config[CONF_FULL_UPDATE_CLEAR_MODE]))

    cg.add_library("FastEPD", None, FASTEPD_REPOSITORY)
    cg.add_library("Wire", None)
