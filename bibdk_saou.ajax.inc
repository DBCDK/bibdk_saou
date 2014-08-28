<?php

/** Ajax page callback
 *
 * @param null $pid
 * @param null $url
 */
function bibdk_saou_ajax($pid=NULL, $url=NULL){
  $pid = $_POST['pid'];

 // $pid = '810010-katalog:008202024 ';

  $url = isset($_POST['url']) ? $_POST['url'] : NULL;
  //use case: pid starts with 870970-basis and no relationurl is given
  if(strpos($pid, '870970-basis') === 0 && empty($url)){
    $output = bibdk_saou_ajax_fetchLicensByPid($pid);
    drupal_json_output($output);
    drupal_exit();
  }
  elseif(!empty($url)){
    $output = bibdk_saou_fetchLicenseByUrl($pid, $url);
    drupal_json_output($output);
    drupal_exit();
  }
  else{
    // default behoviour
    $output = bibdk_saou_ajax_fetchLicensByPid($pid);
    drupal_json_output($output);
    drupal_exit();
  }
}

/** Do fetchLicenseByUrl request and pass response to output function
 * @param $pid
 * @param $url
 * @return array, render array from output function
 */
function bibdk_saou_fetchLicenseByUrl($pid, $url){
  $agencyAndUserIds = bibdk_saout_get_agency_and_userids($agencyId);
  $response = open_saou_FetchLicenseByUrl($pid, $url, $agencyAndUserIds,  $agencyId);
  $saou = new SaouResponse($response);
  return bibdk_saou_get_output_by_permission($saou, $pid);
}

/** Dp fetchLicenseByPid request, and pass response to output function.
 * @param $pid
 * @return array, render array from output function
 */
function bibdk_saou_ajax_fetchLicensByPid($pid){
  $agencyAndUserIds = bibdk_saout_get_agency_and_userids($agencyId);
  $response = open_saou_FetchLicenseByPid($pid,$agencyAndUserIds,  $agencyId);
  $saou = new SaouResponse($response);
  return bibdk_saou_get_output_by_permission($saou, $pid);
}

/** get orderagency from bibdk_favourites.
 * set given agencyId by reference
 *
 * @param $agencyId NOTICE by reference
 * @return array agencyAndUserId => [agencyId, userId]
 */
function bibdk_saout_get_agency_and_userids(&$agencyId){
  $agency = bibdk_saou_get_order_agency();
  $agencyId = $agency->getAgencyId();
  $agencyAndUserIds = array('agencyAndUserId'=>array('agencyId'=>$agency->getAgencyId(), 'userId' =>$agency->getUserId()));
  return $agencyAndUserIds;
}

/** Create the render arrays.
 *
 * @param SaouResponse object, $saou
 * @param string $pid
 * @return array, Render array az expected by drupal_render
 */
function bibdk_saou_get_output_by_permission($saou, $pid){
  $permission = $saou->get_remoteUserAccess();
  if(!isset($permission)){

    // get the manifestation to display default link
    $manifestation = bibdk_saou_get_manifestation($pid);
    $txt = $manifestation->getInfotext();
    $url = $manifestation->getSingleAccessUrl();

    $value['accessUrl'] = current($url);
    $value['accessUrlDisplay'] =  t('bibdk_saou_no_license',array(),array('context'=>'bibdk_saou'));

    $element[] = $value;
    $link = _ting_openformat_parse_element($element);


    // no licenseinfo - permission is NULL
    $message = $link;
    $output = bibdk_saou_field_get_element_to_toggle($message);
    bibdk_saou_remove_hidden_class($output['container']['#attributes']['class']);
    $html = drupal_render($output);
    $output = array('error'=>$html, 'selector' => bibdk_saou_field_pid_to_selector($pid));
   // $output = array('error'=>$markup, 'selector' => bibdk_saou_field_pid_to_selector($pid));
    return $output;
  }
  if($permission){
    // permission is ok
    $link = $saou->get_proxyLink();
    if($link){
      $output = bibdk_saou_ajax_get_proxylink($link, $pid);
      return array('redirect'=>$output,'selector' => bibdk_saou_field_pid_to_selector($pid),'error'=>'');
    }
    else{
      // this is an error from waou service.
      $message = 'internal error: no link to proxy';
      $output = bibdk_saou_field_get_element_to_toggle($message);
      bibdk_saou_remove_hidden_class($output['container']['#attributes']['class']);
      $html = drupal_render($output);
      $output = array('error'=>$html, 'selector' => bibdk_saou_field_pid_to_selector($pid));
      return $output;
    }
  }
  else{
    // permission is FALSE
    $message =  t('bibdk_saou_no_remote_access_for_library',array(),array('context'=>'bibdk_saou'));
    $output = bibdk_saou_field_get_element_to_toggle($message);
    bibdk_saou_remove_hidden_class($output['container']['#attributes']['class']);
    $html = drupal_render($output);
    $output = array('error'=>$html, 'selector' => bibdk_saou_field_pid_to_selector($pid));
    return $output;
  }
}

/** Remove hidden class for toggle element. It should be shown after ajax callback
 * @param array $classes
 */
function bibdk_saou_remove_hidden_class(array &$classes){
  foreach($classes as $key => $class){
    if($class == 'hidden'){
      unset($classes[$key]);
      break;
    }
  }
}

/** Define a link to proxy.
 *
 * @param string $link, url
 * @param string $pid
 * @return string,html as ouptput by drupal_render
 */
function bibdk_saou_ajax_get_proxylink($link, $pid){
  $proxy_link = array(
    '#theme' => 'link',
    '#text' => $link,
    '#path' => $link,
    '#options' => array(
      'attributes' => array('target'=>'_blank', 'id' => bibdk_saou_field_pid_to_selector($pid)),
      'html' => FALSE,
      'absolute => TRUE',
    ),
  );
  return drupal_render($proxy_link);
}
