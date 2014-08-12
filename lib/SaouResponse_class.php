<?php
class SaouResponse{
  private $response;
  private $licenseInfo;

  public function __construct(stdClass $response){
//print_r($response);
    $this->response = $response;
    $this->licenseInfo = $response->Licenses->License->licenseInfo;

  }

  public function getlicenseInfo(){
    return isset($this->licenseInfo) ? $this->licenseInfo : FALSE;
  }

  public function get_ipAccess(){
    return isset($this->licenseInfo->ipAccess) ? $this->licenseInfo->ipAccess : FALSE;
  }

  public function get_remoteUserAccess(){
    return isset($this->getlicenseInfo()->remoteUserAccess) ? $this->getlicenseInfo()->remoteUserAccess : FALSE ;
  }

  public function get_linkResponseType(){}

  public function get_proxyLink(){
    return isset ($this->response->Licenses->License->linkResponseType->proxyLink) ? $this->response->Licenses->License->linkResponseType->proxyLink : FALSE;
  }
}