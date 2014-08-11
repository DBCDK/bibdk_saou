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
    }
  }
  return $element;
}

function bibdk_soau_field_get_link($entity){
  $message = bibdk_saou_get_user_message($entity);
  $link = array(
    'link'=>array(
      'message' => $message,
      'text' => t($entity->getRestrictedUse(),array(),array('context'=>'bibdk_saou')),
    ),
  );
  return theme('bibdk_saou_link',$link);


  // first use case .. user is not logged in- redirect to login
  /*$link = array(
    '#href'         => _ting_infomedia_get_href($id, $user_status, $relation_uri),
    '#title'        => $title,
    '#popup'        => (($user_status == 'user_is_infomedia_user') ? 'bibdk-popup-link ' : ''),
    '#popover'      => (($user_status == 'user_is_infomedia_user') ? '' : 'popover-button '),
    '#data-pid'     => $id,
    '#user-message' => _ting_infomedia_get_user_message($user_status, $relation_uri),
    '#weight' => 0,
  );*/

  return $link;
}

function bibdk_saou_get_user_message($entity){
  if(!ding_user_is_provider_user()){
    return bibdk_saou_link_to_login();
  }
}

function bibdk_saou_link_to_login($entity){
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