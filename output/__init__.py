import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_CHANNEL, CONF_ID
from .. import TLC59283, tlc59283_ns

DEPENDENCIES = ["tlc59283"]

TLC59283Channel = tlc59283_ns.class_(
    "TLC59283Channel", output.FloatOutput, cg.Parented.template(TLC59283)
)

CONF_TLC59283_ID = "tlc59283_id"
CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_TLC59283_ID): cv.use_id(TLC59283),
        cv.Required(CONF_ID): cv.declare_id(TLC59283Channel),
        cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=65535),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await output.register_output(var, config)
    await cg.register_parented(var, config[CONF_TLC59283_ID])
    cg.add(var.set_channel(config[CONF_CHANNEL]))
