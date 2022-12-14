<?php
//session_start();
include_once 'defensaconsumidor.helpers.inc';
include_once 'defensaconsumidor_validate.inc';
include_once 'defensaconsumidor_config.inc';

function argentinagobar_defensaconsumidor_form($form, &$form_state, $no_js_use = FALSE) {

     if(empty($_SESSION['appSession'])){
        
        $status = new ConsultaVUFDCStatus(ApiGatewayAuth::getInstance());
        $_SESSION['appSession'] = $status->consultar();
    }
    
    
    if(isset($_SESSION['appSession'])){
          
    $consulta = new ConsultaGeoRefProvincias(ApiGatewayAuth::getInstance());
    $provincias = $consulta->get_select_options();

    $form['#name'] = 'argentinagobar_defensaconsumidor';
    $form['#attributes'] = array('autocomplete' => array('off')); // Eliminar los autocompletes del agent
    // Async evita el cross site scripting
    $form['#attached']['js'][] = drupal_get_path("module", "argentinagobar_formularios").'/includes/form/defensa_consumidor/async.js';

    // DATOS PERSONALES

    $form['personales'] = array(
        '#type' => 'fieldset',
        '#title' => '<h3>Tus datos personales</h3>',
    );

    $form['personales']['nombre'] = array(
        '#type' => 'textfield',
        '#title' => t('Nombre'),
        '#size' => 30,
        '#maxlength' => 50,
        '#required' => TRUE,
        '#default_value' => autocompletarCiudadanoDigital('given_name'),
        '#rules' => array(
            array('rule' => 'texto_con_espacios', 'error' => 'El nombre debe ser un texto de al menos tres letras'),
        ),
    );

    $block = module_invoke('argentinagobar_ciudadano_digital', 'completarDatosMiArgentina', 'block_delta');
    print render($block['content']);

    $form['personales']['apellido'] = array(
        '#type' => 'textfield',
        '#title' => t('Apellido'),
        '#size' => 30,
        '#maxlength' => 100,
        '#required' => TRUE,
        '#default_value' => autocompletarCiudadanoDigital('family_name'),
        '#rules' => array(
            array('rule' => 'texto_con_espacios', 'error' => 'El nombre debe ser un texto de al menos tres letras'),
        ),
    );

    $active = array('DNI' => t('DNI'), 'LC' => t('LC'), 'LE' => t('LE'), 'CI' => t('CI'), 'Pasaporte' => t('Pasaporte'));
    $form['personales']['tipo_doc'] = array(
        '#prefix' => '<div class="col-md-4" style="padding-left:0;">',
        '#suffix' => '</div>',
        '#type' => 'select',
        '#title' => t('Tipo de documento'),
        '#options' => $active,
        '#group' => 'consulta',
        '#required' => TRUE,
        '#default_value' => !empty(autocompletarCiudadanoDigital('dni_type')) ? autocompletarCiudadanoDigital('dni_type') : 'DNI',
        '#default_value' => 'DNI',
    );

    $form['personales']['doc'] = array(
        '#prefix' => '<div class="col-md-8" style="padding-right:0;">',
        '#suffix' => '</div>',
        '#type' => 'textfield',
        '#title' => t('N??mero de documento'),
        '#size' => 30,
        '#maxlength' => 10,
        '#required' => TRUE,
        '#default_value' => !empty(autocompletarCiudadanoDigital('dni_number')) ? autocompletarCiudadanoDigital('dni_number') : '',
    );

    $active = array('F' => t('Femenino'), 'M' => t('Masculino'));
    $form['personales']['genero'] = array(
        '#type' => 'radios',
        '#title' => 'Sexo',
        '#field_prefix' => '<div class="text-muted text-normal">Complet?? este dato seg??n lo que figura en tu documento</div>',
        '#options' => $active,
        '#default_value' => autocompletarCiudadanoDigital('gender'),
        '#group' => 'consulta',
        '#theme' => array('form_element_radios_btn'),
        '#required' => TRUE,
    );

    $form['personales']['fechaNacimiento'] = array(
        '#type' => 'textfield',
        '#title' => t('Fecha de nacimiento'),
        '#required' => TRUE,
        '#attributes' => array(
            'class'=>array('dateMaskStyler'),
            'placeholder'=>'dd/mm/aaaa',
        ),
        '#default_value' => autocompletarCiudadanoDigital('birthdate'),
        '#rules' => array(
            array('rule' => 'regexp[/[0-3][0-9]\/[0-1][0-9]\/[1-2][0-9]{3}/]', 'error' => 'La fecha ingresada no es v??lida'),
          ),
    );

    $form['personales']['domicilio'] = array(
        '#type' => 'textfield',
        '#title' => 'Domicilio',
        '#field_prefix' => '<div class="text-muted text-normal">Coloc?? calle, n??mero, piso y departamento</div>',
        '#size' => 30,
        '#maxlength' => 255,
        '#required' => TRUE,
    );

    $options = array(
        'nombre' => '',
        'caption' => '',
        'required' => TRUE,
        'fieldset' => 'personales',
    );

    argentinagobar_localidad($form, $form_state, $options, $provincias);

    $form['personales']['cp'] = array(
        '#suffix' => '<label class="row" style="margin-top:-20px;"><a class="col-md-12" href="http://www.correoargentino.com.ar/formularios/cpa" target="_blank">Consult?? tu c??digo postal</a></div>',
        '#type' => 'textfield',
        '#title' => t('C??digo postal'),
        '#size' => 30,
        '#maxlength' => 20,
        '#required' => TRUE,
        '#rules' => array(
            array('rule' => 'length[3, 20]', 'error' => 'El c??digo postal es un n??mero o c??digo entre 3 y 20 caracteres'),
          ),
    );

    //////////////////////////////// DATOS DE CONTACTO ///////////////////////////////////////////

    $form['contacto'] = array(
        '#type' => 'fieldset',
        '#title' => '<h3>Datos de contacto<br><span class="text-muted text-normal" style="font-size:16px;">Complet?? al menos una manera para comunicarnos</span></h3><hr>',
        );

        $form['contacto']['email'] = array(
            '#type' => 'textfield',
            '#title' => 'Correo electr??nico',
            '#size' => 30,
            '#maxlength' => 100,
            '#default_value' => autocompletarCiudadanoDigital('email'),
        '#required' => true,
    //		'#states' => array(
    //			'required' => array(
    //				'input[name=reclamopos\\[elegirOrganismo\\]]' => array('value' => 'Servicio de Conciliaci??n previa en las Relaciones de Consumo (COPREC)'),
    //				),
    //			),
            '#rules' => array(
                array('rule' => 'email', 'error' => 'El correo electr??nico no es v??lido'),
              ),
        );

    $form['contacto']['tel_mark'] = array(
        '#markup' => '<div class="form-item col-md-12" style="padding-left:0;"><label class="control-label">Tel??fono</label></div>',
    );

    $form['contacto']['telefono_prefijo'] = array(
        '#prefix' => '<div class="col-md-3" style="padding-left:0;">',
        '#suffix' => '</div>',
        '#type' => 'textfield',
        '#title' => t('Prefijo'),
        '#size' => 30,
        '#maxlength' => 5,
//		'#states' => array(
//			'required' => array(
//				'input[name=contacto\\[email\\]]' => array('value' => ''),
//				),
//			),
        '#rules' => array(
                array('rule' => 'numeric', 'error' => 'El prefijo telef??nico s??lo puede contener n??meros'),
                array('rule' => 'length[3, 5]', 'error' => 'El prefijo telef??nico debe contener un m??nimo de 3 n??meros y un m??ximo de 5'),
        ),
    );

    $form['contacto']['telefono'] = array(
        '#prefix' => '<div class="col-md-9" style="padding-right:0;">',
        '#suffix' => '</div>',
        '#type' => 'textfield',
        '#title' => t('N??mero'),
        '#size' => 30,
        '#maxlength' => 30,
//		'#states' => array(
//			'required' => array(
//				'input[name=contacto\\[email\\]]' => array('value' => ''),
//				),
//			),
    );



    //////////////////////////////// DATOS DEL RECLAMO ///////////////////////////////////////////

    $form['reclamo'] = array(
        '#type' => 'fieldset',
        '#title' => '<h3>Datos del reclamo</h3>',
        );

    $form['#validate'] = array();

    $active = array(1 => t('S??'), 0 => t('No') );
    $form['reclamo']['esTitular'] = array(
        '#type' => 'radios',
        '#title' => t('??Sos el titular del producto o servicio?'),
        '#default_value' => 1,
        '#options'=>$active,
        '#theme' => array('form_element_radios_btn'),
        '#required' => TRUE,
        );

    $form['reclamo']['nombreTitular'] = array(
        '#type' => 'textfield',
        '#title' => t('Ingres?? el nombre y apellido del titular'),
        '#size' => 30,
        '#states' => array(
            'visible' => array(
                'input[name=reclamo\\[esTitular\\]]' => array('value' => 0),
                ),
            ),
        '#maxlength' => 255,
    );

    $active = array(1 => t('S??'), 0 => t('No') );
    $form['reclamo']['esUsuario'] = array(
        '#type' => 'radios',
        '#title' => '??Us??s o usaste el producto o servicio? / ??Sos el usuario? *',
        '#default_value' => 1,
        '#options'=>$active,
        '#theme' => array('form_element_radios_btn'),
        '#states' => array(
            'visible' => array(
                'input[name=reclamo\\[esTitular\\]]' => array('value' => 0),
                ),
            'required' => array(
                'input[name=reclamo\\[esTitular\\]]' => array('value' => 1),
                ),
            ),
        '#field_suffix' => '<div class="text-muted text-normal">Si la respuesta es no ten??s que acreditar tu condici??n con un poder. Vas a recibir las notificaciones cuando tu reclamo est?? en curso</div><br>',
    );

    // Motivo del reclamo
    $form['reclamo']['motivo'] = array(
        '#type' => 'textarea',
        '#title' => '??Cu??l es el problema?',
        '#required' => TRUE,
        //'#default_value' => 'El feroz murci??lago come trigo en un trigal',
        '#field_prefix' => '<div class="text-muted text-normal">Describ?? el hecho y el motivo de tu reclamo. Por ejemplo: adquir?? un celular en el comercio ???X" y a la semana dej?? de funcionar. Lo llev?? al servicio t??cnico y no me dan respuesta. Tambi??n puede ser: estoy sin internet hace un mes y medio, llam?? 4 veces a la compa????a y siguen sin solucionarlo.</div>',
        '#rules' => array(
            array('rule' => 'length[20, 5000]', 'error' => 'Desarrolle el motivo de su reclamo de 20 a 5000 caracteres'),
            ),
        );

    $active = array(
        'Cambio de producto.' => t('Cambio de producto'),
        'Bonificaci??n en el abono.' => t('Bonificaci??n en el abono'),
        'Compensaci??n de mis gastos.' => t('Compensaci??n de mis gastos'),
        'Reparaci??n del producto / servicio t??cnico.' => t('Reparaci??n del producto / servicio t??cnico'),
        'Anulaci??n del contrato.' => t('Anulaci??n del contrato'),
        'Devoluci??n del dinero.' => t('Devoluci??n del dinero'),
        'Otro.' => t('Otro')
        );
    $form['reclamo']['solicito'] = array(
        '#type' => 'checkboxes',
        '#title' => '??C??mo esper??s que se resuelva?',
        '#field_prefix' => '<div class="text-muted text-normal">Pod??s seleccionar m??s de una opci??n.</div>',
        '#required' => TRUE,
        '#options'=>$active,
        );

    $form['reclamo']['otraPretencion'] = array(
        '#type' => 'textarea',
        '#title' => t('Explic?? las pretensiones de tu reclamo'),
        '#size' => 30,
        '#states' => array(
            'visible' => array(
                ':input[name="reclamo[solicito][Otro.]"]' => array('checked' => TRUE),
                ),
            ),
    );

    $active = array(1 => t('S??'), 0 => t('No') );
    $form['reclamo']['previoProveedor'] = array(
        '#type' => 'radios',
        '#title' => t('??Ya realizaste el reclamo al proveedor del producto o servicio?'),
        '#default_value' => null,
        '#options'=>$active,
        '#theme' => array('form_element_radios_btn'),
        '#required' => TRUE,
    );

    $form['reclamo']['numeroProveedor'] = array(
        '#type' => 'textfield',
        '#title' => t('Ingres?? el n??mero de reclamo'),
        '#size' => 30,
        '#states' => array(
            'visible' => array(
                'input[name=reclamo\\[previoProveedor\\]]' => array('value' => 1),
                ),
            ),
        '#maxlength' => 255,
    );


    $active = array(1 => t('S??'), 0 => t('No') );
    $form['reclamo']['previoOrganismo'] = array(
        '#type' => 'radios',
        '#title' => t('??Ya realizaste el reclamo ante un organismo gubernamental?'),
        '#default_value' => null,
        '#options'=>$active,
        '#theme' => array('form_element_radios_btn'),
        '#required' => TRUE,
    );

    $form['reclamo']['dondeOrganismo'] = array(
        '#type' => 'textfield',
        '#title' => t('??En cu??l?'),
        '#size' => 30,
        '#states' => array(
            'visible' => array(
                'input[name=reclamo\\[previoOrganismo\\]]' => array('value' => 1),
                ),
            'required' => array(
                'input[name=reclamo\\[previoOrganismo\\]]' => array('value' => 1),
                ),
            ),
        '#maxlength' => 100,
    );

    $form['reclamo']['numeroOrganismo'] = array(
        '#type' => 'textfield',
        '#title' => t('Ingres?? n??mero de reclamo'),
        '#size' => 30,
        '#states' => array(
            'visible' => array(
                'input[name=reclamo\\[previoOrganismo\\]]' => array('value' => 1),
                ),
            ),
        '#maxlength' => 100,
    );

    $form['reclamo']['dondeOrganismo'] = array(
        '#type' => 'textfield',
        '#title' => t('??En cu??l?'),
        '#size' => 30,
        '#states' => array(
            'visible' => array(
                'input[name=reclamo\\[previoOrganismo\\]]' => array('value' => 1),
                ),
            'required' => array(
                'input[name=reclamo\\[previoOrganismo\\]]' => array('value' => 1),
                ),
            ),
        '#maxlength' => 100,
    );

    $options = array(
        'caption' => '??D??nde compraste el producto o contrataste el servicio?',
        'required' => TRUE,
        'fieldset' => 'reclamo'
    );

    argentinagobar_localidad($form, $form_state, $options, $provincias);

    $options = array(
        'caption' => '??D??nde us??s o usaste el producto o servicio?',
        'required' => FALSE,
        'fieldset' => 'uso'
    );

    argentinagobar_localidad($form, $form_state, $options, $provincias);

    $active = array(1 => t('S??'), 0 => t('No') );
    $form['archivoscondicional']['tiene_documentacion'] = array(
        '#type' => 'radios',
        '#title' => t('??Ten??s una factura u otra documentaci??n que sirva como prueba?'),
        '#default_value' => null,
        '#options'=>$active,
        '#required' => TRUE,
        '#theme' => array('form_element_radios_btn'),
        '#field_suffix' => '<div class="text-muted text-normal" id="claim_documentacion" style="display:none;">Record?? que ten??s que llevar los originales a la audiencia.</div><br>'
        );

    $active = array(1 => t('S??'), 0 => t('No') );
    $form['archivoscondicional']['pruebas'] = array(
        '#type' => 'radios',
        '#title' => '??Quer??s adjuntar la documentaci??n ahora?',
        '#field_prefix' => '<div class="text-muted text-normal">Pod??s adjuntar hasta 10 documentos de tipo pdf, word, jpg y png.</div>',
        '#default_value' => null,
        '#options'=>$active,
        '#required' => TRUE,
        '#theme' => array('form_element_radios_btn'),
        );

    $form['#tree'] = TRUE;
    $form['archivos'] = array(
        '#type' => 'fieldset',
        '#title' => '<h3>Documentaci??n que se agrega al reclamo</h3>',
        '#prefix' => '<div id="names-fieldset-wrapper">',
        '#suffix' => '</div>',
        '#states' => array(
            'visible' => array(
                'input[name=archivoscondicional\\[pruebas\\]]' => array('value' => 1),
                ),
            ),
        );

    // Agrega campos din??micos

    if ( empty($form_state['narchivos']) ) {
        $form_state['narchivos'] = 10;
    }

    for ( $i = 0; $i < $form_state['narchivos']; $i++ ) {

        $doc='<h5 style="margin-top:0px;">Documento N?? '.($i+1).'</h5>';

            $form['archivos']['documento'.$i] = array(
                '#prefix' => ($i>0)?'<div id="doc'.$i.'" style="display:none;">'.$doc : '<div id="doc'.$i.'">'.$doc,
                '#suffix' => '</div>',
                '#name' => 'documento'.$i,
                '#type' => 'managed_file',
                '#description' => t('The uploaded image will be displayed on this page using the image style choosen below.'),
                '#upload_validators' => array(
                    'file_validate_extensions' => array('pdf png jpg jpeg doc docx'),
                    'file_validate_size' => array(2048*1024),
                 ),
            );

            $form['archivos']['documento'.$i]['#title'] = 'Documento '.($i+1);

            if ($i>0) {

                $form['archivos']['documento'.$i]['#states'] = array(
                    'visible' => array(
                        'input[name=files\\[archivos_documento'.($i-1).'\\]]' => array('!value' => ''),
                    )
                );
            }

    }

    // DATOS PROVEEDOR

    $form['proveedor'] = array(
        '#type' => 'fieldset',
        '#title' => '<h3>Datos del proveedor del producto o servicio<br><span class="text-muted text-normal" style="font-size:16px;">Esta informaci??n la pod??s sacar de tu factura o ticket.</span></h3>',
        );

    // Proveedores din??micos
    if ( empty($form_state['nproveedores']) ) {
        $form_state['nproveedores'] = 5;
    }

    for ( $i = 1; $i <= $form_state['nproveedores']; $i++ ) {

        $required = ($i<2) ? TRUE : FALSE;

        $form['proveedor'.$i] = array(
            '#prefix' => ($i>1)?'<div id="prov'.$i.'" style="display:none;">' : '<div id="prov'.$i.'">',
            '#suffix' => '</div>',
            '#type' => 'fieldset',
            '#title' => '<h3>Datos del proveedor '.$i.'</h3>',
        );

        $form['proveedor'.$i]['nombre'] = array(
            '#type' => 'textfield',
            '#title' => 'Nombre del proveedor (empresa o negocio)',
            '#size' => 30,
            '#maxlength' => 255,
            '#required' => $required,
            '#rules' => array(
                array('rule' => 'length[5, 50]', 'error' => 'El nombre del proveedor debe contener entre 5 y 50'),
              ),
            );

        $form['proveedor'.$i]['cuit'] = array(
            '#type' => 'textfield',
            '#title' => t('N??mero de CUIT'),
            '#size' => 30,
            '#maxlength' => 11,
            '#rules' => array(
                array('rule' => 'numeric', 'error' => 'El CUIT es un n??mero no coloque guiones ni comas'),
                array('rule' => 'length[10, 11]', 'error' => 'El CUIT es un n??mero entre 10 y 11 caracteres'),
              ),
            );

        // AGREGAR SERVICIO DE CONSULTA DE CUIL

        $form['proveedor'.$i]['direccion'] = array(
        '#type' => 'textfield',
        '#title' => t('Domicilio'),
            '#size' => 30,
            '#maxlength' => 255,
            '#required' => $required,
            '#rules' => array(
                array('rule' => 'regexp[/^[a-zA-Z]{2,50}.*/]', 'error' => 'La direcci??n debe comenzar con el nombre de la calle'),
              ),
            );

        $options = array(
            'title' => 'Lugar'.$i,
            'caption' => '',
            'required' => $required,
            'fieldset' => 'proveedor'.$i,
        );

        argentinagobar_localidad($form, $form_state, $options, $provincias);


    }
    $form['submit']['#type'] = 'submit';
    $form['submit']['#value'] = t('Finalizar y enviar reclamo');

    $options = array (
        'title' => 'Complet?? el formulario',
        'subtitle' => '(Los datos que tienen un asterisco * son obligatorios)',
        'response' => TRUE,
        'progress' => FALSE,
        'ajax' => FALSE
    );

    argentinagobar_formstyler($form, $options);

    if(isset($form_state['respuesta'])){
        // Si recibis la respuesta ok reemplaz??s el formulario de lo contrario lo preserv??s con el mensaje
        if (isset($form_state['respuesta']['id_respuesta'])) {
            $form = array();
            $form['respuesta']['mostrar'] = array(
                '#markup' => $form_state['respuesta']['respuesta'],
            );
        } else {
            array_unshift($form, array('#markup' => $form_state['respuesta']['respuesta'].'<div class="clearfix"></div>'));
        }
    }

    $form['submit']['#attributes'] = array('class' => array('btn btn-success btn-block form-submit'));

    // Deshabilita el boton en caso de no cargar provincias
    $form['submit']['#disabled'] = $form['submit_disabled'];
    unset($form['submit_disabled']);
    if(isset($form['submit_tooltip'])){
        $form['submit']['#attributes']['title'] = array($form['submit_tooltip']);
        unset($form['submit_tooltip']);
    }

    } else {

        $form['contact_information'] = array(
              '#markup' => '<div class="alert alert-warning"><h3>Moment??neamente este formulario est?? en mantenimiento y no se pueden hacer reclamos por este medio.</h3><p>El formulario estar?? disponible a la brevedad.</p></div>',
        );

    }

    return $form;
}

function argentinagobar_defensaconsumidor_form_submit($form, &$form_state) {

    $opciones = variable_get('DC_opciones');
    $documents = array();

    foreach ($form_state['values']['archivos'] as $fid) {
        if(!($fid == 0)):

          $form_state['values']['documento'] = $fid;
          $consulta = new ConsultaVUFDCUploadFile(ApiGatewayAuth::getInstance(), $form_state['values']);
          $documents[] = $consulta->consultar();

        endif;
    }

    $postIssue = new ConsultaDefensaDelConsumidor(ApiGatewayAuth::getInstance());
    $postIssue->parse_x_www_form_urlencoded($form_state['values'], implode(',', $documents));
    $response = $postIssue->consultar();

    $form_state['respuesta'] = $response;
    $form_state['rebuild'] = TRUE;

}

function argentinagobar_defensaconsumidor_validar($form, $form_state){

    $val = $form_state['values']['personales'];

    _validarFechaNacimiento($val['fechaNacimiento']);

    _validarComprobanteDC($form_state['values']['reclamo']['previoOrganismo'], $form_state['values']['reclamo']['dondeOrganismo']);

    _validarAlmenosUnDatoDeContacto($form_state['values']['contacto']);

}

function lists_session($key, $value = NULL) {
  static $storage;
  if ($value) {
    $storage[$key] = $value ;
    $_SESSION['lists'][$key] = $value ;   // I use 'lists' in case some other module uses 'type' in $_SESSION
  }
  else if (empty($storage[$key]) && isset($_SESSION['lists'][$key])) {
    $storage[$key] = $_SESSION['lists'][$key];
  }
  return $storage[$key];
}