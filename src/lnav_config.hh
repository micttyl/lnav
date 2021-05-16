/**
 * Copyright (c) 2013, Timothy Stack
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * * Neither the name of Timothy Stack nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @file lnav_config.hh
 */

#ifndef lnav_config_hh
#define lnav_config_hh

#include <map>
#include <set>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

#include "base/result.h"
#include "base/file_range.hh"
#include "log_level.hh"
#include "styling.hh"
#include "ghc/filesystem.hpp"

#include "lnav_config_fwd.hh"
#include "archive_manager.cfg.hh"
#include "file_vtab.cfg.hh"
#include "logfile.cfg.hh"

/**
 * Check if an experimental feature should be enabled by
 * examining the LNAV_EXP environment variable.
 *
 * @param feature_name The feature name to check for in
 *   the LNAV_EXP environment variable.
 *
 * @return True if the feature was mentioned in the env
 *   var and should be enabled.
 */
bool check_experimental(const char *feature_name);

/**
 * Ensure that the '.lnav' directory exists.
 */
void ensure_dotlnav();

bool install_from_git(const char *repo);
bool update_installs_from_git();

void install_extra_formats();

struct key_command {
    std::string kc_cmd;
    std::string kc_alt_msg;
};

struct key_map {
    std::map<std::string, key_command> km_seq_to_cmd;
};

struct _lnav_config {
    std::string lc_ui_clock_format;
    bool lc_ui_dim_text;
    bool lc_ui_default_colors{true};
    std::string lc_ui_keymap;
    std::string lc_ui_theme;
    std::unordered_map<std::string, key_map> lc_ui_keymaps;
    std::map<std::string, std::string> lc_ui_key_overrides;
    std::map<std::string, std::string> lc_global_vars;
    std::map<std::string, lnav_theme> lc_ui_theme_defs;

    key_map lc_active_keymap;

    archive_manager::config lc_archive_manager;
    file_vtab::config lc_file_vtab;
    lnav::logfile::config lc_logfile;
};

extern struct _lnav_config lnav_config;
extern struct _lnav_config rollback_lnav_config;
extern std::map<intern_string_t, source_location> lnav_config_locations;

extern struct json_path_container lnav_config_handlers;

enum class config_file_type {
    FORMAT,
    CONFIG,
};

Result<config_file_type, std::string> detect_config_file_type(const ghc::filesystem::path &path);

void load_config(const std::vector<ghc::filesystem::path> &extra_paths,
                 std::vector<std::string> &errors);

void reset_config(const std::string &path);

void reload_config(std::vector<std::string> &errors);

std::string save_config();

extern const char *DEFAULT_FORMAT_SCHEMA;
extern std::set<std::string> SUPPORTED_FORMAT_SCHEMAS;

#endif
