<?php
/**
 * @file
 * argentinagobar_cck.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function argentinagobar_cck_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function argentinagobar_cck_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}
