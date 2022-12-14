<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Texto Migración'),
  'description' => t('Crea un párrafo.'),
  'category' => t('Argentina'),
  'edit form' => 'argentinagobar_migtram_content_texto_edit_form',
  'render callback' => 'argentinagobar_migtram_content_texto_render',
  'admin info' => 'argentinagobar_migtram_content_texto_admin_info',
  'defaults' => array(
    'destacado' => 0,
    'texto' => NULL,
    'sobreescribir' => 0,
    'cuerpo_o_resumen' => 'body'
  )
);

function argentinagobar_migtram_content_texto_edit_form($form, &$form_state){

  $form['destacado'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Texto destacado'),
  );
  $form['cuerpo_o_resumen'] = array(
    '#type' => 'radios',
    '#title' => t('Cantidad de texto'),
    '#default_value' => $form_state['conf']['cuerpo_o_resumen'],
    '#options' => array('body' => 'Texto completo', 'summary' => 'Resumen'),
  );
  $form['sobreescribir'] = array(
    '#title' => t('¿Personalizar texto?'),
    '#type' => 'checkbox',
    '#default_value' => $form_state['conf']['sobreescribir'],
  );
  $form['texto'] = array(
    '#title' => t('Contenido'),
    '#type' => 'text_format',
    //'#format' => 'full_html',
    '#default_value' => $form_state['conf']['texto']['value'],
    '#required' => FALSE,
    '#states' => array(
      'disabled' => array(
        ':input[name="sobreescribir"]' => array('checked' => FALSE),
      ),
      'required' => array(
        ':input[name="sobreescribir"]' => array('checked' => TRUE),
      )
    ),
  );

  $form['#attached']['js'][] = drupal_get_path("module", "argentinagobar_migtram_contenido")."/js/disableCKEditor.js" ;
  return $form;
}

/**
* The submit form stores the data in $conf.
*/
function argentinagobar_migtram_content_texto_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

function argentinagobar_migtram_content_texto_render($subtype, $conf, $panel_args, $context = NULL){
  
  $block = new stdClass();
  $block->title = '';
  $texto = $conf['sobreescribir'] ? $conf['texto'] : (esNodo() ? field_get_items('node', node_load(arg(1)), 'body', NULL) : '');
  $texto = $conf['sobreescribir'] ? $texto['value'] : ($conf['cuerpo_o_resumen'] == 'body' ? $texto[0]['safe_value'] : resumen($texto));

  $texto = $conf['texto']['format'] == 'full_html' ? $texto : check_markup($texto,$conf['texto']['format']);

  $block->content = theme('html_tag', array(
    'element' => array(
      '#tag' => 'div',
      '#attributes' => array(
        'class' => $conf['destacado'] ? 'lead' : '',
        ),
      '#value' => $texto,
    ),
  ));

  return $block;
}

function argentinagobar_migtram_content_texto_admin_info($subtype, $conf, $contexts){
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : t('Texto');
    $block->content = t('@texto.', array('@texto' => $conf['texto']));
    return $block;
  }
}
function _nodoDisponible($sobreescribir){
  return (!$sobreescribir && arg(0) == 'node' && is_numeric(arg(1)));
}
function _resumen($campo){
  return empty($campo[0]['safe_summary']) ? views_trim_text(array('max_length' => 200, 'ellipsis' => TRUE, 'word_boundary' => TRUE,'html' => TRUE), $campo[0]['safe_value']) : $campo[0]['safe_summary'];
}