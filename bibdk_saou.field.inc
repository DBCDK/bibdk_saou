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
    // check if entity has fields with restricted access
    if( $entity->getRestrictedUse() === 'restrictedAccess') {
      $link = bibdk_soau_field_get_link($entity);
    }
  }
  return $element;
}

function bibdk_soau_field_get_link($entity){
  dpm($entity);
}