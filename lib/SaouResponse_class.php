cd pocasdfasfasdfasfasdf<?php
/** Class to handle response from saou webservice.
 * Basically you get two kinds of responses
 *
 * Either yóu get <responseCode>OK_NO_LICENSES</responseCode> which means
 * that SAOU has no licenses
 *
 * Or you get <Licenses> which means that one or more licenses are returned.
 *
 **/
class SaouResponse{
  private $response;
  private $licenseInfo;

  /** Constructor
   * @param stdClass $response; Response from SAOU webservice
   */
  public function __construct(stdClass $response){
    $this->response = $response;
    $this->licenseInfo = isset($response->Licenses->License->licenseInfo) ?
    $response->Licenses->License->licenseInfo : NULL;
  }

  /** Get info for licenses
   * @return mixed; array licenseinfo if licenses are found; FALSE if not
   */
  public function getlicenseInfo(){
    return isset($this->licenseInfo) ? $this->licenseInfo : FALSE;
  }

  /** Get ipAccess for license
   * @return bool
   */
  public function get_ipAccess(){
    return isset($this->licenseInfo->ipAccess) ? $this->licenseInfo->ipAccess : FALSE;
  }

  /** Get remote user access
   * @return mixed; BOOL if license is given; NULL if not
   */
  public function get_remoteUserAccess(){
    return isset($this->getlicenseInfo()->remoteUserAccess) ? $this->getlicenseInfo()->remoteUserAccess : NULL ;
  }

  /** Get proxy link to ressource
   * @return mixed; string proxyurl if given; FALSE if not
   **/
  public function get_proxyLink(){
    return isset ($this->response->Licenses->License->linkResponseType->proxyLink) ? $this->response->Licenses->License->linkResponseType->proxyLink : FALSE;
  }
}