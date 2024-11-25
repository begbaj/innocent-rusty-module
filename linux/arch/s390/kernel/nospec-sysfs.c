// SPDX-License-Identifier: GPL-2.0
#include <linux/device.h>
#include <linux/cpu.h>
#include <asm/facility.h>
#include <asm/nospec-branch.h>

ssize_t cpu_show_spectre_v1(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "Mitigation: __user pointer sanitization\n");
}

ssize_t cpu_show_spectre_v2(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	if (test_facility(156))
		return sprintf(buf, "Mitigation: etokens\n");
	if (nospec_uses_trampoline())
		return sprintf(buf, "Mitigation: execute trampolines\n");
	if (nobp_enabled())
		return sprintf(buf, "Mitigation: limited branch prediction\n");
	return sprintf(buf, "Vulnerable\n");
}
