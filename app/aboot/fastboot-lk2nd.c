#include <string.h>
#include <debug.h>
#include <pm8x41_regulator.h>
#include <libfdt.h>
#include <lk2nd-device.h>
#include "fastboot.h"

#if TARGET_MSM8916
static void cmd_oem_dump_regulators(const char *arg, void *data, unsigned sz)
{
	char response[MAX_RSP_SIZE];
	struct spmi_regulator *vreg;
	for (vreg = target_get_regulators(); vreg->name; ++vreg) {
		snprintf(response, sizeof(response), "%s: enabled: %d, voltage: %d mV",
			 vreg->name, regulator_is_enabled(vreg),
			 regulator_get_voltage(vreg));
		fastboot_info(response);
	}
	fastboot_okay("");
}
#endif

#if WITH_DEBUG_LOG_BUF
static void cmd_oem_lk_log(const char *arg, void *data, unsigned sz)
{
	fastboot_stage(lk_log_getbuf(), lk_log_getsize());
}
#endif

static void cmd_oem_dtb(const char *arg, void *data, unsigned sz)
{
	fastboot_stage(lk2nd_dev.fdt, fdt_totalsize(lk2nd_dev.fdt));
}

static void cmd_oem_pstore(const char *arg, void *data, unsigned sz)
{
	fastboot_stage(lk2nd_dev.pstore, lk2nd_dev.pstore_size);
}

static void cmd_oem_cmdline(const char *arg, void *data, unsigned sz)
{
	fastboot_stage(lk2nd_dev.cmdline, strlen(lk2nd_dev.cmdline));
}

void fastboot_lk2nd_register_commands(void) {
#if TARGET_MSM8916
	fastboot_register("oem dump-regulators", cmd_oem_dump_regulators);
#endif
#if WITH_DEBUG_LOG_BUF
	fastboot_register("oem lk_log", cmd_oem_lk_log);
#endif

	if (lk2nd_dev.fdt)
		fastboot_register("oem dtb", cmd_oem_dtb);

	if (lk2nd_dev.pstore)
		fastboot_register("oem pstore", cmd_oem_pstore);

	if (lk2nd_dev.cmdline)
		fastboot_register("oem cmdline", cmd_oem_cmdline);
}
