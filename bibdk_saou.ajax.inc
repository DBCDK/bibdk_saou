<?php

/** Ajax page callback
 *
 * @param null $pid
 * @param null $url
 */
function bibdk_saou_ajax($pid=NULL, $url=NULL){
  //print_r(func_get_args());
  // for now hardcode pid
 // $pid = '810010-katalog:008202024';

  $pid = $_POST['pid'];
  $url = isset($_POST['url']) ? $_POST['url'] : NULL;


 // var_dump($url);
 // var_dump($pid);

  //use case: pid starts with 870970-basis and no relationurl is given
  if(strpos($pid, '870970-basis') === 0 && empty($url)){
    // use FetchLicenseByPid method
    // get order agency for request
    $output = bibdk_saou_ajax_fetchLicensByPid($pid);

    drupal_json_output($output);
    drupal_exit();
  }
  elseif(strpos($pid, '870970-basis') === 0 && !empty($url)){
    // use fetchLicenseByUrl method
    // @TDOO implement.
    // we need an axeample
  }
  else{
    // default behoviour
    $output = bibdk_saou_ajax_fetchLicensByPid($pid);
    drupal_json_output($output);
    drupal_exit();
  }
}

function bibdk_saou_ajax_fetchLicensByPid($pid){
  $agency = bibdk_saou_get_order_agency();
  $agencyAndUserIds = array('agencyAndUserId'=>array('agencyId'=>$agency->getAgencyId(), 'userId' =>$agency->getUserId()));
  $response = open_saou_FetchLicenseByPid($pid,$agencyAndUserIds,  $agency->getAgencyId());
  $saou = new SaouResponse($response);

  $permission = $saou->get_remoteUserAccess();
 if($permission){
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
    // favourite library has no remoteUserAccess to this ressource
    $message =  t('bibdk_saou_no_remote_access_for_library',array(),array('context'=>'bibdk_saou'));
    $output = bibdk_saou_field_get_element_to_toggle($message);
    bibdk_saou_remove_hidden_class($output['container']['#attributes']['class']);
    $html = drupal_render($output);
    $output = array('error'=>$html, 'selector' => bibdk_saou_field_pid_to_selector($pid));
    return $output;
  }
}

function bibdk_saou_remove_hidden_class(array &$classes){
  foreach($classes as $key => $class){
    if($class == 'hidden'){
      unset($classes[$key]);
      break;
    }
  }
}



function bibdk_saou_ajax_get_proxylink($link, $pid){
  $link = array(
    '#theme' => 'link',
    '#text' => $link,
    '#path' => $link,
    '#options' => array(
      'attributes' => array('target'=>'_blank', 'id' => bibdk_saou_field_pid_to_selector($pid)),
      'html' => FALSE,
      'absolute => TRUE',
    ),
  );
  return drupal_render($link);
}
