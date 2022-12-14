<?php
/**
 * @file
 * field_group.group_tramites_relacionados_node_tramite_form.inc
 */

$api = '2.0.0';

$data = $field_group = new stdClass();
$field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
$field_group->api_version = 1;
$field_group->identifier = 'group_tramites_relacionados|node|tramite|form';
$field_group->group_name = 'group_tramites_relacionados';
$field_group->entity_type = 'node';
$field_group->bundle = 'tramite';
$field_group->mode = 'form';
$field_group->parent_name = 'group_general';
$field_group->data = array(
  'label' => 'Tramites Relacionados',
  'weight' => '19',
  'children' => array(
    0 => 'field_servicios_relacionados',
  ),
  'format_type' => 'fieldset',
  'format_settings' => array(
    'formatter' => 'collapsible',
    'instance_settings' => array(
      'description' => '',
      'classes' => 'group-tramites-relacionados field-group-fieldset',
      'required_fields' => 1,
      'id' => '',
    ),
  ),
);


$dependencies = array();

$optional = array();

$modules = array(
  0 => 'ctools',
  1 => 'field_group',
);
