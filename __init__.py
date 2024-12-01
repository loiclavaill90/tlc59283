# this component is for the "TLC59283 24-Channel, 16-Bit PWM LED Driver" [https://www.ti.com/lit/ds/symlink/tlc59283.pdf]

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import (
    CONF_CLOCK_PIN,
    CONF_DATA_PIN,
    CONF_ID,
    CONF_NUM_CHIPS,
)


CODEOWNERS = ["@loiclavaill90"]

tlc59283_ns = cg.esphome_ns.namespace("tlc59283")
TLC59283 = tlc59283_ns.class_("TLC59283", cg.Component)

MULTI_CONF = True
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(TLC59283),
        cv.Required(CONF_DATA_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_CLOCK_PIN): pins.gpio_output_pin_schema,
        cv.Optional(CONF_NUM_CHIPS, default=1): cv.int_range(min=1, max=85),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    data = await cg.gpio_pin_expression(config[CONF_DATA_PIN])
    cg.add(var.set_data_pin(data))
    clock = await cg.gpio_pin_expression(config[CONF_CLOCK_PIN])
    cg.add(var.set_clock_pin(clock))

    cg.add(var.set_num_chips(config[CONF_NUM_CHIPS]))
