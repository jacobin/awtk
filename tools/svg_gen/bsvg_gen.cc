/**
 * File:   bsvg_gen.cc
 * Author: AWTK Develop Team
 * Brief:  bsvg_gen
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-11-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/fs.h"
#include "tkc/path.h"
#include "common/utils.h"
#include "base/assets_manager.h"
#include "svg/svg_to_bsvg.h"

static ret_t bsvg_gen(const char* input_file, const char* output_file, const char* theme, const char *name,
                      bool_t bin) {
  uint32_t* out = NULL;
  uint32_t size = 0;
  uint32_t out_size = 0;
  char* xml = (char*)file_read(input_file, &size);

  if (svg_to_bsvg(xml, size, &out, &out_size) == RET_OK) {
    if (bin) {
      write_file(output_file, out, out_size);
    } else {
      output_res_c_source_ex(output_file, theme, ASSET_TYPE_IMAGE, ASSET_TYPE_IMAGE_BSVG,
                          (uint8_t*)out, out_size, name);
    }
  }

  TKMEM_FREE(xml);

  return RET_OK;
}

static ret_t gen_one(const char* input_file, const char* output_file, const char* theme, const char *name,
                     bool_t bin) {
  ret_t ret = RET_OK;
  if (!exit_if_need_not_update(input_file, output_file)) {
    ret = bsvg_gen(input_file, output_file, theme, name, bin);
    if (ret == RET_FAIL) {
      GEN_ERROR(input_file);
    }
  }
  return ret;
}

static ret_t gen_folder(const char* in_foldername, const char* out_foldername, const char* theme, const char* dir_name,
                        bool_t bin) {
  fs_item_t item;
  ret_t ret = RET_OK;
  char in_name[MAX_PATH] = {0};
  char out_name[MAX_PATH] = {0};
  fs_dir_t* dir = fs_open_dir(os_fs(), in_foldername);

  while (fs_dir_read(dir, &item) != RET_FAIL) {
    if (item.is_reg_file && case_end_with(item.name, ".svg")) {
      str_t str_name;
      str_t res_name;
      char ext_array[MAX_PATH] = {0};
      path_extname(item.name, ext_array, MAX_PATH);

      str_init(&res_name, 0);
      str_init(&str_name, 0);
      str_set(&str_name, item.name);
      str_replace(&str_name, ext_array, "");
      if (!bin) {
        filter_name(str_name.str);
      }

      str_append(&res_name, dir_name);
      str_append(&res_name, item.name);
      str_replace(&res_name, ".svg", "");

      str_append(&str_name, ".bsvg");
      path_build(in_name, MAX_PATH, in_foldername, item.name, NULL);
      path_build(out_name, MAX_PATH, out_foldername, str_name.str, NULL);
      ret = gen_one(in_name, out_name, theme, res_name.str, bin);
      str_reset(&str_name);
      str_reset(&res_name);
      if (ret == RET_FAIL) {
        break;
      }
    } else if (item.is_dir && !tk_str_eq(item.name, ".") && !tk_str_eq(item.name, "..")) {
      str_t res_name;
      path_build(in_name, MAX_PATH, in_foldername, item.name, NULL);
      path_build(out_name, MAX_PATH, out_foldername, item.name, NULL);

      if (!fs_dir_exist(os_fs(), out_name)) {
        fs_create_dir(os_fs(), out_name);
      }
      
      str_init(&res_name, 0);
      str_append(&res_name, item.name);
      str_append(&res_name, "/");
      ret = gen_folder(in_name, out_name, theme, res_name.str, bin);
      str_reset(&res_name);
      if (ret != RET_OK) {
        break;
      }
    }
  }
  fs_dir_close(dir);
  return ret;
}

int wmain(int argc, wchar_t* argv[]) {
  bool_t output_bin = argc == 4;
  const char* in_filename = NULL;
  const char* out_filename = NULL;

  platform_prepare();

  if (argc < 3) {
    printf("Usage: %S svg_filename bsvg_filename [bin]\n", argv[0]);
    return 0;
  }

  str_t in_file;
  str_t out_file;
  str_t theme_name;

  str_init(&in_file, 0);
  str_init(&out_file, 0);

  str_from_wstr(&in_file, argv[1]);
  str_from_wstr(&out_file, argv[2]);

  in_filename = in_file.str;
  out_filename = out_file.str;

  str_init(&theme_name, 0);
  if (argc > 4) {
    str_from_wstr(&theme_name, argv[4]);
  }

  fs_stat_info_t in_stat_info;
  fs_stat_info_t out_stat_info;
  fs_stat(os_fs(), in_filename, &in_stat_info);
  fs_stat(os_fs(), out_filename, &out_stat_info);
  if (in_stat_info.is_dir == TRUE && out_stat_info.is_dir == TRUE) {
    gen_folder(in_filename, out_filename, theme_name.str, "", output_bin);
  } else if (in_stat_info.is_reg_file == TRUE) {
    char name[MAX_PATH + 1] = {0};
    path_basename_ex(in_filename, TRUE, name, sizeof(name));
    gen_one(in_filename, out_filename, theme_name.str, name, output_bin);
  } else {
    GEN_ERROR(in_filename);
  }

  str_reset(&in_file);
  str_reset(&out_file);
  str_reset(&theme_name);
  return 0;
}

#include "common/main.inc"
