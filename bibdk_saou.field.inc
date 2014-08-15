<?php
/**
 * @file
 * remaining field definitions for the bibdk_saou field
 *
 * field_info and field_load hooks are in the bibdk_saou_field module to enable
 * deletion of field
 *
 * see http://drupal.org/node/1285540
 */

/**
 * Implements hook_field_formatter_info().
 */
function bibdk_saou_field_formatter_info() {
  return array(
    'bibdk_saou_link_default' => array(
      'label' => t('Default'),
      'field types' => array(
        'bibdk_saou_link',
      ),
    )
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function bibdk_saou_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  foreach ($items as $delta => $item) {
    // check if entity has fields with restricted access and is a link
    if( $entity->getRestrictedUse() === 'restrictedAccess' && $entity->getRequestMethod() == 'link') {
      $link = bibdk_soau_field_get_link($entity);
      $element[$delta] = $link;
      $element['#attached']['js'][] = drupal_get_path('module', 'bibdk_saou') . '/js/bibdk_saou.js';

    }
  }
  return $element;
}


// @TODO make message a link to favourites
function bibdk_soau_field_check_favourites(){
  $favourites = bibdk_favourite_get_favourites();
  $message = '';
  if(empty($favourites)){
    $message = t('bibdk_saou_no_fvaourites',array(),array('context'=>'bibdk_saou'));
  }
  else{
    $fav = current($favourites);
    $userid = $fav->getUserId();
    if(empty($userid)){
      $message = t('bibdk_saou_incomplete_favorite_data',array(),array('context'=>'bibdk_saou'));
    }
  }
  return $message;
}

function bibdk_soau_field_get_link($entity){
  $message = bibdk_saou_get_user_message();

  if(empty($message)){
    $output['wrapper'] =bibdk_saou_field_get_dropdown_wrapper();
    $output['wrapper']['link'] = bibdk_saou_link_to_soau_ressource($entity);
  }
  else{
    $output = bibdk_saou_field_get_link_and_toggle_element($entity, $message);
  }
  return drupal_render($output);
}

function bibdk_saou_field_get_link_and_toggle_element($entity, $message){
  $output['wrapper'] = bibdk_saou_field_get_dropdown_wrapper();
  $output['wrapper']['link'] = bibdk_saou_field_get_message_link($entity);
  $output['wrapper']['toggle'] = bibdk_saou_field_get_element_to_toggle($message);

  return $output;
}

function bibdk_saou_field_get_message_link($entity){
  $text = t($entity->getRestrictedUse(),array(),array('context'=>'bibdk_saou'));
  $link = array(
    '#theme' => 'link',
    '#text' => $text,
    '#path' => '#',
    '#options' => array(
      'attributes' => array(
        'data-pid' => $entity->id,
        'class' => array('dropdown-toggle'),
        'id' => 'bibdk_saou_'.$entity->id,
      ),
      'html' => FALSE,
    ),
  );
  return $link;
}

function bibdk_saou_field_get_dropdown_wrapper(){
  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('dropdown-wrapper'),
    )
  );
}

function bibdk_saou_get_user_message(){
  if(!ding_user_is_provider_user()){
    return bibdk_saou_link_to_login();
  }
  else{
    return bibdk_soau_field_check_favourites();
  }
}

function bibdk_saou_field_get_element_to_toggle($message){
  $element = array();
  if(empty($message)){
    return $element;
  }

  $element['container'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('dropdown-menu','btn', 'hidden'),
    )
  );
  $element['container']['text'] = array(
    '#markup' => $message,
  );

  return $element;
}

/** Render array for link to soau ressource
 * @param $entity
 * @return array
 */
function bibdk_saou_link_to_soau_ressource($entity){
  $message = t($entity->getRestrictedUse(),array(),array('context'=>'bibdk_saou'));
  $path = 'saou/ajax';
  $uri = $entity->getRelationUri();

  $link = array(
    '#theme' => 'link',
    '#text' => $message,
    '#path' => $path,
    '#options' => array(
      'attributes' => array(
        'data-pid' => $entity->id,
        'data-url' => ($uri) ? $uri : 'NONE',
        'class' => array('soau-ressource-link'),
        'id' => 'bibdk_saou_'.bibdk_saou_field_pid_to_selector($entity->id),
      ),

      'html' => FALSE,
    ),
  );
  return $link;
}

function bibdk_saou_field_pid_to_selector($pid){
  $pattern = array('/-/', '/:/');
  $replacement =  array('_','_');
  return preg_replace($pattern,$replacement,$pid);

}

function bibdk_saou_link_to_login(){
  $message = t('saou_log_in_to_accesss_ressource',array(),array('context'=>'bibdk_saou'));
  $path = 'user/login';
  $link = array(
    '#theme' => 'link',
    '#text' => $message,
    '#path' => $path,
    '#options' => array(
      'attributes' => array(),
      'html' => FALSE,
    ),
  );

  return drupal_render($link);
}