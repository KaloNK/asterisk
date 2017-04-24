/*
 * Asterisk -- An open source telephony toolkit.
 *
 * Copyright (C) 2017, Digium, Inc.
 *
 * Mark Michelson <mmichelson@digium.com>
 *
 * See http://www.asterisk.org for more information about
 * the Asterisk project. Please do not directly contact
 * any of the maintainers of this project for assistance;
 * the project provides a web site, mailing lists and IRC
 * channels for your use.
 *
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 */

#ifndef _ASTERISK_SDP_STATE_H
#define _ASTERISK_SDP_STATE_H

#include "asterisk/stream.h"
#include "asterisk/sdp_options.h"

struct ast_sdp_state;
struct ast_sockaddr;

/*!
 * \brief Allocate a new SDP state
 *
 * SDP state keeps tabs on everything SDP-related for a media session.
 * Most SDP operations will require the state to be provided.
 * Ownership of the SDP options is taken on by the SDP state.
 * A good strategy is to call this during session creation.
 */
struct ast_sdp_state *ast_sdp_state_alloc(struct ast_stream_topology *streams,
	struct ast_sdp_options *options);

/*!
 * \brief Free the SDP state.
 *
 * A good strategy is to call this during session destruction
 */
void ast_sdp_state_free(struct ast_sdp_state *sdp_state);

/*!
 * \brief Get the associated RTP instance for a particular stream on the SDP state.
 *
 * Stream numbers correspond to the streams in the topology of the associated channel
 */
struct ast_rtp_instance *ast_sdp_state_get_rtp_instance(const struct ast_sdp_state *sdp_state,
	int stream_index);

/*!
 * \brief Get the global connection address on the SDP state.
 */
const struct ast_sockaddr *ast_sdp_state_get_connection_address(const struct ast_sdp_state *sdp_state);

/*!
 * \brief Get the connection address for a particular stream.
 *
 * \param sdp_state
 * \param stream_index The particular stream to get the connection address of
 * \param address[out] A place to store the address in
 *
 * \retval 0 Success
 *
 * \note
 * Stream numbers correspond to the streams in the topology of the associated channel
 */
int ast_sdp_state_get_stream_connection_address(const struct ast_sdp_state *sdp_state,
	int stream_index, struct ast_sockaddr *address);

/*!
 * \brief Get the joint negotiated streams based on local and remote capabilities.
 *
 * If this is called prior to receiving a remote SDP, then this will just mirror
 * the local configured endpoint capabilities.
 */
const struct ast_stream_topology *ast_sdp_state_get_joint_topology(
	const struct ast_sdp_state *sdp_state);

/*!
 * \brief Get the local topology
 *
 */
const struct ast_stream_topology *ast_sdp_state_get_local_topology(
	const struct ast_sdp_state *sdp_state);

/*!
 * \brief Get the sdp_state options
 *
 */
const struct ast_sdp_options *ast_sdp_state_get_options(
	const struct ast_sdp_state *sdp_state);


/*!
 * \brief Get the local SDP.
 *
 * \param sdp_state
 *
 * \retval non-NULL Success
 * \retval NULL Failure
 *
 * \note
 * This function will allocate a new SDP with RTP instances if it has not already
 * been allocated.
 *
 */
const struct ast_sdp *ast_sdp_state_get_local_sdp(struct ast_sdp_state *sdp_state);

/*!
 * \brief Get the local SDP Implementation.
 *
 * \param sdp_state
 *
 * \retval non-NULL Success
 * \retval NULL Failure
 *
 * \note
 * This function calls ast_sdp_state_get_local_sdp then translates it into
 * the defined implementation.
 *
 * The return here is const. The use case for this is so that a channel can add
 * the SDP to an outgoing message. The API user should not attempt to modify the SDP.
 * SDP modification should only be done through the API.
 *
 * \since 15
 */
const void *ast_sdp_state_get_local_sdp_impl(struct ast_sdp_state *sdp_state);

/*!
 * \brief Set the remote SDP
 *
 * \param sdp_state
 * \param sdp
 *
 * \since 15
 */
void ast_sdp_state_set_remote_sdp(struct ast_sdp_state *sdp_state, struct ast_sdp *sdp);

/*!
 * \brief Set the remote SDP from an Implementation
 *
 * \param sdp_state
 * \param remote The implementation's representation of an SDP.
 *
 * \retval 0 Success
 * \retval non-0 Failure
 *
 * \since 15
 */
int ast_sdp_state_set_remote_sdp_from_impl(struct ast_sdp_state *sdp_state, void *remote);

/*!
 * \brief Reset the SDP state and stream capabilities as if the SDP state had just been allocated.
 *
 * \param sdp_state
 * \param remote The implementation's representation of an SDP.
 *
 * \retval 0 Success
 *
 * \note
 * This is most useful for when a channel driver is sending a session refresh message
 * and needs to re-advertise its initial capabilities instead of the previously-negotiated
 * joint capabilities.
 *
 * \since 15
 */
int ast_sdp_state_reset(struct ast_sdp_state *sdp_state);

/*!
 * \brief Update the local stream topology on the SDP state.
 *
 * \param sdp_state
 * \param streams The new stream topology.
 *
 * \retval 0 Success
 *
 * \since 15
 */
int ast_sdp_state_update_local_topology(struct ast_sdp_state *sdp_state, struct ast_stream_topology *streams);

/*!
 * \brief Set the local address (IP address) to use for connection addresses
 *
 * \param sdp_state
 * \param address The local address
 *
 * \note
 * Passing NULL as an address will unset the explicit local connection address.
 *
 * \since 15
 */
void ast_sdp_state_set_local_address(struct ast_sdp_state *sdp_state, struct ast_sockaddr *address);

/*!
 * \brief Set the connection address (IP address and port) to use for a specific stream
 *
 * \param sdp_state
 * \param stream_index The stream to set the connection address for
 * \param address The connection address
 *
 * \retval 0 Success
 *
 * \note
 * Passing NULL as an address will unset the explicit local connection address.
 *
 * \since 15
 */
int ast_sdp_state_set_connection_address(struct ast_sdp_state *sdp_state, int stream_index,
	struct ast_sockaddr *address);

/*!
 * \since 15.0.0
 * \brief Set a stream to be held or unheld
 *
 * \param sdp_state
 * \param stream_index The stream to set the held value for
 * \param locally_held
 */
void ast_sdp_state_set_locally_held(struct ast_sdp_state *sdp_state,
	int stream_index, unsigned int locally_held);

/*!
 * \since 15.0.0
 * \brief Get whether a stream is held or not
 *
 * \param sdp_state
 * \param stream_index The stream to get the held state for
 *
 * \returns locally_held
 */
unsigned int ast_sdp_state_get_locally_held(const struct ast_sdp_state *sdp_state,
	int stream_index);

#endif /* _ASTERISK_SDP_STATE_H */
