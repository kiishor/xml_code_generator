#ifndef FACET_H
#define FACET_H

/**
 * \file
 * \brief Helper function to set the facet properties

 * \author  Nandkishor Biradar
 * \date    12 August 2022

 *  Copyright (c) 2019-2022 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE or copy at https://mit-license.org/)
 */

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */

extern void* get_facet_max_target(const xml_content_t* const content);
extern void* get_facet_min_target(const xml_content_t* const content);
extern void exclude_facet_max_value(xml_content_t* const content);
extern void exclude_facet_min_value(xml_content_t* const content);

#endif // FACET_H
