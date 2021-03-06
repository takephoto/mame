// license:BSD-3-Clause
// copyright-holders:Nigel Barnes
/**********************************************************************

    ALA11 - Acorn Plus 1

    The Acorn Plus 1 added two ROM cartridge slots, an analogue interface
    (supporting two channels) and a Centronics parallel port. The
    analogue interface was normally used for joysticks, the parallel for
    a printer. The ROM slots could be booted from via the "Shift+BREAK"
    key-combination. (The slot at the front of the interface took priority
    if both were populated).

**********************************************************************/


#include "emu.h"
#include "plus1.h"


//**************************************************************************
//  DEVICE DEFINITIONS
//**************************************************************************

DEFINE_DEVICE_TYPE(ELECTRON_PLUS1, electron_plus1_device, "electron_plus1", "Acorn Plus 1 Expansion")


//-------------------------------------------------
//  ROM( plus1 )
//-------------------------------------------------

ROM_START( plus1 )
	// Bank 12 Expansion module operating system
	ROM_REGION( 0x2000, "exp_rom", 0 )
	ROM_DEFAULT_BIOS("plus1")
	ROM_SYSTEM_BIOS(0, "plus1", "Expansion 1.00")
	ROMX_LOAD("plus1.rom", 0x0000, 0x1000, CRC(ac30b0ed) SHA1(2de04ab7c81414d6c9c967f965c53fc276392463), ROM_BIOS(1))
	ROM_RELOAD(            0x1000, 0x1000)

	ROM_SYSTEM_BIOS(1, "presap1", "PRES Expansion 1.1")
	ROMX_LOAD("presplus1.rom", 0x0000, 0x1000, CRC(8ef1e0e5) SHA1(080e1b788b3fe4fa272cd2cc792293eb7b874e82), ROM_BIOS(2))
	ROM_RELOAD(                0x1000, 0x1000)

	ROM_SYSTEM_BIOS(2, "presap2", "PRES AP2 Support 1.23")
	ROMX_LOAD("presap2_123.rom", 0x0000, 0x2000, CRC(f796689c) SHA1(bc40a79e6d2b4cb5e549d5d21f673c66a661850d), ROM_BIOS(3))

	ROM_SYSTEM_BIOS(3, "sl200", "Slogger Expansion 2.00")
	ROMX_LOAD("elkexp200.rom", 0x0000, 0x2000, CRC(dee02843) SHA1(5c9b940b4ddb46e9a223160310683a32266300c8), ROM_BIOS(4))

	ROM_SYSTEM_BIOS(4, "sl201", "Slogger Expansion 2.01")
	ROMX_LOAD("elkexp201.rom", 0x0000, 0x2000, CRC(0e896892) SHA1(4e0794f1083fe529b01bd4fa100996a533ed8b10), ROM_BIOS(5))

	ROM_SYSTEM_BIOS(5, "sl202", "Slogger Expansion 2.02")
	ROMX_LOAD("elkexp202.rom", 0x0000, 0x2000, CRC(32b440be) SHA1(dbc73e8d919c5615d0241d99db60e06324e16c86), ROM_BIOS(6))
ROM_END

//-------------------------------------------------
//  INPUT_PORTS( plus1 )
//-------------------------------------------------

static INPUT_PORTS_START( plus1 )
	PORT_START("JOY1")
	PORT_BIT(0xff, 0x80, IPT_AD_STICK_X) PORT_SENSITIVITY(100) PORT_KEYDELTA(100) PORT_CENTERDELTA(100) PORT_MINMAX(0x00, 0xff) PORT_PLAYER(1) PORT_REVERSE

	PORT_START("JOY2")
	PORT_BIT(0xff, 0x80, IPT_AD_STICK_Y) PORT_SENSITIVITY(100) PORT_KEYDELTA(100) PORT_CENTERDELTA(100) PORT_MINMAX(0x00, 0xff) PORT_PLAYER(1) PORT_REVERSE

	PORT_START("JOY3")
	PORT_BIT(0xff, 0x80, IPT_AD_STICK_X) PORT_SENSITIVITY(100) PORT_KEYDELTA(100) PORT_CENTERDELTA(100) PORT_MINMAX(0x00, 0xff) PORT_PLAYER(2) PORT_REVERSE

	PORT_START("JOY4")
	PORT_BIT(0xff, 0x80, IPT_AD_STICK_Y) PORT_SENSITIVITY(100) PORT_KEYDELTA(100) PORT_CENTERDELTA(100) PORT_MINMAX(0x00, 0xff) PORT_PLAYER(2) PORT_REVERSE

	PORT_START("BUTTONS")
	PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_BUTTON1) PORT_PLAYER(1)
	PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_BUTTON1) PORT_PLAYER(2)
INPUT_PORTS_END

//-------------------------------------------------
//  machine_config_additions - device-specific
//  machine configurations
//-------------------------------------------------

ioport_constructor electron_plus1_device::device_input_ports() const
{
	return INPUT_PORTS_NAME( plus1 );
}

//-------------------------------------------------
//  device_add_mconfig - add device configuration
//-------------------------------------------------

MACHINE_CONFIG_MEMBER( electron_plus1_device::device_add_mconfig )
	/* printer */
	MCFG_CENTRONICS_ADD("centronics", centronics_devices, "printer")
	MCFG_CENTRONICS_BUSY_HANDLER(WRITELINE(electron_plus1_device, busy_w))
	MCFG_CENTRONICS_OUTPUT_LATCH_ADD("cent_data_out", "centronics")

	/* adc */
	MCFG_ADC0844_ADD("adc")
	MCFG_ADC0844_INTR_CB(WRITELINE(electron_plus1_device, ready_w))
	MCFG_ADC0844_CH1_CB(IOPORT("JOY1"))
	MCFG_ADC0844_CH2_CB(IOPORT("JOY2"))
	MCFG_ADC0844_CH3_CB(IOPORT("JOY3"))
	MCFG_ADC0844_CH4_CB(IOPORT("JOY4"))

	/* cartridges */
	MCFG_GENERIC_CARTSLOT_ADD("cart_sk1", generic_plain_slot, "electron_cart")
	MCFG_GENERIC_LOAD(electron_plus1_device, electron_cart_sk1)
	MCFG_GENERIC_CARTSLOT_ADD("cart_sk2", generic_plain_slot, "electron_cart")
	MCFG_GENERIC_LOAD(electron_plus1_device, electron_cart_sk2)
MACHINE_CONFIG_END

//-------------------------------------------------
//  rom_region - device-specific ROM region
//-------------------------------------------------

const tiny_rom_entry *electron_plus1_device::device_rom_region() const
{
	return ROM_NAME( plus1 );
}

//**************************************************************************
//  LIVE DEVICE
//**************************************************************************

//-------------------------------------------------
//  electron_plus1_device - constructor
//-------------------------------------------------

electron_plus1_device::electron_plus1_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: device_t(mconfig, ELECTRON_PLUS1, tag, owner, clock),
		device_electron_expansion_interface(mconfig, *this),
	m_exp_rom(*this, "exp_rom"),
	m_cart_sk1(*this, "cart_sk1"),
	m_cart_sk2(*this, "cart_sk2"),
	m_centronics(*this, "centronics"),
	m_cent_data_out(*this, "cent_data_out"),
	m_adc(*this, "adc"),
	m_joy(*this, "JOY%u", 1),
	m_buttons(*this, "BUTTONS"),
	m_centronics_busy(0),
	m_adc_ready(0)
{
}

//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void electron_plus1_device::device_start()
{
	address_space& space = machine().device("maincpu")->memory().space(AS_PROGRAM);
	m_slot = dynamic_cast<electron_expansion_slot_device *>(owner());

	space.install_readwrite_handler(0xfc70, 0xfc70, READ8_DEVICE_DELEGATE(m_adc, adc0844_device, read), WRITE8_DEVICE_DELEGATE(m_adc, adc0844_device, write));
	space.install_write_handler(0xfc71, 0xfc71, WRITE8_DEVICE_DELEGATE("cent_data_out", output_latch_device, write));
	space.install_read_handler(0xfc72, 0xfc72, READ8_DELEGATE(electron_plus1_device, status_r));
}

//-------------------------------------------------
//  device_reset - device-specific reset
//-------------------------------------------------

void electron_plus1_device::device_reset()
{
	std::string region_tag;
	memory_region *tmp_reg;
	if (m_cart_sk2 && (tmp_reg = memregion(region_tag.assign(m_cart_sk2->tag()).append(GENERIC_ROM_REGION_TAG).c_str())))
	{
		machine().root_device().membank("bank2")->configure_entries(0, 2, tmp_reg->base(), 0x4000);
	}
	if (m_cart_sk1 && (tmp_reg = memregion(region_tag.assign(m_cart_sk1->tag()).append(GENERIC_ROM_REGION_TAG).c_str())))
	{
		machine().root_device().membank("bank2")->configure_entries(2, 2, tmp_reg->base(), 0x4000);
	}

	machine().root_device().membank("bank2")->configure_entry(12, memregion("exp_rom")->base());
}

//**************************************************************************
//  IMPLEMENTATION
//**************************************************************************

READ8_MEMBER(electron_plus1_device::status_r)
{
	uint8_t data = 0x0f;
	// Status: b7: printer Busy
	//         b6: ADC conversion end
	//         b5: Fire Button 1
	//         b4: Fire Button 2
	data |= (m_centronics_busy << 7);
	data |= (m_adc_ready << 6);
	data |= m_buttons->read();

	return data;
}

WRITE_LINE_MEMBER(electron_plus1_device::busy_w)
{
	m_centronics_busy = !state;
}

WRITE_LINE_MEMBER(electron_plus1_device::ready_w)
{
	m_adc_ready = !state;
}

image_init_result electron_plus1_device::load_cart(device_image_interface &image, generic_slot_device *slot)
{
	if (image.software_entry() == nullptr)
	{
		uint32_t filesize = image.length();

		if (filesize != 16384)
		{
			image.seterror(IMAGE_ERROR_UNSPECIFIED, "Invalid size: Only size 16384 is supported");
			return image_init_result::FAIL;
		}

		slot->rom_alloc(filesize, GENERIC_ROM8_WIDTH, ENDIANNESS_LITTLE);
		image.fread(slot->get_rom_base(), filesize);
		return image_init_result::PASS;
	}
	else
	{
		int upsize = image.get_software_region_length("uprom");
		int losize = image.get_software_region_length("lorom");

		if (upsize != 16384 && upsize != 0)
		{
			image.seterror(IMAGE_ERROR_UNSPECIFIED, "Invalid size for uprom");
			return image_init_result::FAIL;
		}

		if (losize != 16384 && losize != 0)
		{
			image.seterror(IMAGE_ERROR_UNSPECIFIED, "Invalid size for lorom");
			return image_init_result::FAIL;
		}

		slot->rom_alloc(upsize + losize, GENERIC_ROM8_WIDTH, ENDIANNESS_LITTLE);

		if (upsize)
			memcpy(slot->get_rom_base(), image.get_software_region("uprom"), upsize);

		if (losize)
			memcpy(slot->get_rom_base() + upsize, image.get_software_region("lorom"), losize);

		return image_init_result::PASS;
	}
}
