/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__INTERRUPT_H
#define MEE__INTERRUPT_H

/*! @file interrupt.h
 *  @brief API for registering and manipulating interrupts
 */

#include <stddef.h>

/*!
 * @brief Possible mode of interrupts to operate
 */
typedef enum mee_vector_mode_ {
    MEE_DIRECT_MODE = 0,
    MEE_VECTOR_MODE = 1,
    MEE_SELECTIVE_VECTOR_MODE = 2,
    MEE_HARDWARE_VECTOR_MODE = 3
} mee_vector_mode;

/*!
 * @brief Function signature for interrupt callback handlers
 */
typedef void (*mee_interrupt_handler_t) (int, void *);

struct mee_interrupt;

struct mee_interrupt_vtable {
    void (*interrupt_init)(struct mee_interrupt *controller);
    int (*interrupt_register)(struct mee_interrupt *controller, int id,
			      mee_interrupt_handler_t isr, void *priv_data);
    int (*interrupt_enable)(struct mee_interrupt *controller, int id);
    int (*interrupt_disable)(struct mee_interrupt *controller, int id);
    int (*interrupt_vector_enable)(struct mee_interrupt *controller,
                                   int id, mee_vector_mode mode);
    int (*interrupt_vector_disable)(struct mee_interrupt *controller, int id);
    int (*command_request)(struct mee_interrupt *controller, int cmd, void *data);
};

/*!
 * @brief A handle for an interrupt
 */
struct mee_interrupt {
    const struct mee_interrupt_vtable *vtable;
};

/*!
 * @brief Initialize a given interrupt controller
 *
 * Initialize a given interrupt controller. This function must be called
 * before any interrupts are registered or enabled with the handler. It
 * is invalid to initialize an interrupt controller more than once.
 *
 * @param controller The handle for the interrupt controller
 */
inline void mee_interrupt_init(struct mee_interrupt *controller)
{
    return controller->vtable->interrupt_init(controller);
}


/*!
 * @brief Register an interrupt handler
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to register
 * @param handler The interrupt handler callback
 * @param priv_data Private data for the interrupt handler
 * @return 0 upon success
 */
inline int mee_interrupt_register_handler(struct mee_interrupt *controller,
                                          int id,
                                          mee_interrupt_handler_t handler,
                                          void *priv_data)
{
    return controller->vtable->interrupt_register(controller, id, handler, priv_data);
}

/*!
 * @brief Enable an interrupt
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to enable
 * @return 0 upon success
 */
inline int mee_interrupt_enable(struct mee_interrupt *controller, int id)
{
    return controller->vtable->interrupt_enable(controller, id);
}

/*!
 * @brief Disable an interrupt
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to disable
 * @return 0 upon success
 */
inline int mee_interrupt_disable(struct mee_interrupt *controller, int id)
{
    return controller->vtable->interrupt_disable(controller, id);
}

/*!
 * @brief Enable an interrupt vector
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to enable
 * @param mode The interrupt mode type to enable
 * @return 0 upon success
 */
inline int mee_interrupt_vector_enable(struct mee_interrupt *controller,
                                       int id, mee_vector_mode mode)
{
    return controller->vtable->interrupt_vector_enable(controller, id, mode);
}

/*!
 * @brief Disable an interrupt vector
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to disable
 * @return 0 upon success
 */
inline int mee_interrupt_vector_disable(struct mee_interrupt *controller, int id)
{
    return controller->vtable->interrupt_vector_disable(controller, id);
}

/* Utilities function to controll, manages devices via a given interrupt controller */
inline int _mee_interrupt_command_request(struct mee_interrupt *controller,
					 int cmd, void *data)
{
    return controller->vtable->command_request(controller, cmd, data);
}

#endif
