/* SPDX-License-Identifier: GPL-2.0 */

/* Copyright (c) 2012-2018, The Linux Foundation. All rights reserved.
 * Copyright (C) 2018-2024 Linaro Ltd.
 */
#ifndef _IPA_INTERRUPT_H_
#define _IPA_INTERRUPT_H_

#include <linux/types.h>

struct platform_device;

struct ipa;
struct ipa_interrupt;

enum ipa_irq_id;

/**
 * ipa_interrupt_suspend_enable - Enable TX_SUSPEND for an endpoint
 * @interrupt:		IPA interrupt structure
 * @endpoint_id:	Endpoint whose interrupt should be enabled
 *
 * Note:  The "TX" in the name is from the perspective of the IPA hardware.
 * A TX_SUSPEND interrupt arrives on an AP RX enpoint when packet data can't
 * be delivered to the endpoint because it is suspended (or its underlying
 * channel is stopped).
 */
void ipa_interrupt_suspend_enable(struct ipa_interrupt *interrupt,
				  u32 endpoint_id);

/**
 * ipa_interrupt_suspend_disable - Disable TX_SUSPEND for an endpoint
 * @interrupt:		IPA interrupt structure
 * @endpoint_id:	Endpoint whose interrupt should be disabled
 */
void ipa_interrupt_suspend_disable(struct ipa_interrupt *interrupt,
				   u32 endpoint_id);

/**
 * ipa_interrupt_simulate_suspend() - Simulate TX_SUSPEND IPA interrupt
 * @interrupt:	IPA interrupt structure
 *
 * This calls the TX_SUSPEND interrupt handler, as if such an interrupt
 * had been signaled.  This is needed to work around a hardware quirk
 * that occurs if aggregation is active on an endpoint when its underlying
 * channel is suspended.
 */
void ipa_interrupt_simulate_suspend(struct ipa_interrupt *interrupt);

/**
 * ipa_interrupt_enable() - Enable an IPA interrupt type
 * @ipa:	IPA pointer
 * @ipa_irq:	IPA interrupt ID
 */
void ipa_interrupt_enable(struct ipa *ipa, enum ipa_irq_id ipa_irq);

/**
 * ipa_interrupt_disable() - Disable an IPA interrupt type
 * @ipa:	IPA pointer
 * @ipa_irq:	IPA interrupt ID
 */
void ipa_interrupt_disable(struct ipa *ipa, enum ipa_irq_id ipa_irq);

/**
 * ipa_interrupt_irq_enable() - Enable IPA interrupts
 * @ipa:	IPA pointer
 *
 * This enables the IPA interrupt line
 */
void ipa_interrupt_irq_enable(struct ipa *ipa);

/**
 * ipa_interrupt_irq_disable() - Disable IPA interrupts
 * @ipa:	IPA pointer
 *
 * This disables the IPA interrupt line
 */
void ipa_interrupt_irq_disable(struct ipa *ipa);

/**
 * ipa_interrupt_config() - Configure IPA interrupts
 * @ipa:	IPA pointer
 *
 * Return:	0 if successful, or a negative error code
 */
int ipa_interrupt_config(struct ipa *ipa);

/**
 * ipa_interrupt_deconfig() - Inverse of ipa_interrupt_config()
 * @ipa:	IPA pointer
 */
void ipa_interrupt_deconfig(struct ipa *ipa);

/**
 * ipa_interrupt_init() - Initialize the IPA interrupt structure
 * @pdev:	IPA platform device pointer
 *
 * Return:	Pointer to an IPA interrupt structure, or a pointer-coded error
 */
struct ipa_interrupt *ipa_interrupt_init(struct platform_device *pdev);

/**
 * ipa_interrupt_exit() - Inverse of ipa_interrupt_init()
 * @interrupt:	IPA interrupt structure
 */
void ipa_interrupt_exit(struct ipa_interrupt *interrupt);

#endif /* _IPA_INTERRUPT_H_ */
