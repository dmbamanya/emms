<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/coolbutton.php';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';
require_once 'class/society.php';

$client = new CLIENT($_REQUEST['id']);

$client->data['confidential'] ? WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewClient&id=%s&ico=stp&msg=stp',$client->data['id'])) : '';

if ($_REQUEST['society_id']) {
  $soc = new SOCIETY($_REQUEST['society_id']);
  $client->data['advisor_id']	= $soc->data['advisor_id'];
  $client->data['zone_id']		= $soc->data['zone_id'];
  $client->data['society_id']	= $soc->data['id'];
  $client->data['society']		= $soc->data['name'];
  $client->frostdata[] 			= 'society';
  $client->frostdata[] 			= 'advisor_id';
  $client->frostdata[] 			= 'zone_id';
  }

if ($_REQUEST['activate']) { $client->frostdata = array(); }

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('clientForm','POST');

if (($_REQUEST[activate])||(!($_REQUEST['id']||$_GET['society_id']))) {
  $button['id']	    	= "pickSocietybt";
  $button['href']		= 'javascript:setSociety()';
  $button['alt']		= WEBPAGE::$gt['pickSociety'];
  $button['onClick']	= '';
  $button['ico']		= "filter";
  $picksocietybutton  	= New COOLBUTTON($button);
  $form->addElement('static', 	'pickSociety', 	'', COOLBUTTON::getButton($picksocietybutton));
  }

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('clientForm',WEBPAGE::$gt['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('clientForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);
$form->addElement('hidden',		'activate',		$_REQUEST['activate']);
$form->addElement('hidden',		'society_id',	$client->data['society_id']);

$form->addElement('text',		'nick',			WEBPAGE::$gt['tblClients.nick'],		'class=large');
$form->addElement('text',		'first',		WEBPAGE::$gt['tblClients.first'],		'class=large');
$form->addElement('text',		'middle',		WEBPAGE::$gt['tblClients.middle'],		'class=large');
$form->addElement('text',		'last',			WEBPAGE::$gt['tblClients.last'],		'class=large');
$form->addElement('text',		'birthdate',	WEBPAGE::$gt['tblClients.birthdate'],	'class=large');
$form->addElement('text',		'code',			WEBPAGE::$gt['tblClients.code'],		'class=large');
$form->addElement('text',		'spouse',		WEBPAGE::$gt['tblClients.spouse'],		'class=large');
$form->addElement('text',		'dependants',	WEBPAGE::$gt['tblClients.dependants'],	'class=large');
$form->addElement('text',		'skills',		WEBPAGE::$gt['tblClients.skills'],		'class=large');
$form->addElement('text',       'phone',        WEBPAGE::$gt['tblClients.phone'],      'class=large');
$form->addElement('text',       'mobile',       WEBPAGE::$gt['tblClients.mobile'],      'class=large');
$form->addElement('text',       'email',        WEBPAGE::$gt['tblClients.email'],      'class=large');
$form->addElement('text',		'society',		WEBPAGE::$gt['tblClients.society_id'],	'disabled class=large');
$form->addElement('select',		'education',	WEBPAGE::$gt['tblClients.education'],	(array('-'=>'') + $client->education()),	'class=large');
$form->addElement('select',		'cstatus',		WEBPAGE::$gt['tblClients.cstatus'],		(array('-'=>'') + $client->cstatus()),		'class=large');
$form->addElement('select',		'zone_id',		WEBPAGE::$gt['tblClients.zone_id'],		(array('-'=>'') + $client->zones()),		'class=large onChange="javascript:unsetSociety()"');
$form->addElement('select',		'advisor_id',	WEBPAGE::$gt['tblClients.advisor_id'],	(array('-'=>'') + $client->advisors()),		'class=large onChange="javascript:unsetSociety()"');

$form->addElement('textarea',	'memo',			WEBPAGE::$gt['tblClients.memo'], 		'class=large');
$form->addElement('textarea',   'address',         WEBPAGE::$gt['tblClients.address'],        'class=large');

$gender_radio[] = $form->createElement('radio',null,null,WEBPAGE::$gt['tblClients.gender.M'],'M', 'class=radio');
$gender_radio[] = $form->createElement('radio',null,null,WEBPAGE::$gt['tblClients.gender.F'],'F', 'class=radio');
$form->addGroup($gender_radio, 'gender', WEBPAGE::$gt['tblClients.gender'],' ');

//add rules
$form->addRule('first',			sprintf(WEBPAGE::$gt['rangelength'],1,16),'required');
$form->addRule('first',			sprintf(WEBPAGE::$gt['rangelength'],1,16),'rangelength', array(1,16));
$form->addRule('first',			WEBPAGE::$gt['lettersonly'],'lettersonly');
$form->addRule('middle',		sprintf(WEBPAGE::$gt['rangelength'],1,16),'rangelength', array(1,16));
$form->addRule('middle',		WEBPAGE::$gt['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('last',			sprintf(WEBPAGE::$gt['rangelength'],1,32),'required');
$form->addRule('last',			sprintf(WEBPAGE::$gt['rangelength'],1,32),'rangelength', array(1,32));
$form->addRule('last',			WEBPAGE::$gt['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('birthdate',		'yyyy-mm-dd','required');
$form->addRule('birthdate',		'yyyy-mm-dd','regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('code',			WEBPAGE::$gt['required'],'required');
$form->addRule('code',			WEBPAGE::$conf['app']['regex_govID_tip'],'regex',WEBPAGE::$conf['app']['regex_govID']);

$form->registerRule('checkcode', 	'callback', 'checkcode');
$form->addRule('code', 		    WEBPAGE::$gt['codeExists'], 'checkcode', true);

$form->addRule('spouse',		sprintf(WEBPAGE::$gt['rangelength'],1,32),'rangelength', array(1,32));
$form->addRule('spouse',		WEBPAGE::$gt['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('gender',		WEBPAGE::$gt['required'],'required');
$form->addRule('memo',			sprintf(WEBPAGE::$gt['maxlength'],255),'maxlength', 255);
$form->addRule('education',		WEBPAGE::$gt['selectOption'],'required');
$form->addRule('education',		WEBPAGE::$gt['selectOption'],'lettersonly');
$form->addRule('cstatus',		WEBPAGE::$gt['selectOption'],'required');
$form->addRule('cstatus',		WEBPAGE::$gt['selectOption'],'lettersonly');
$form->addRule('advisor_id',	WEBPAGE::$gt['selectOption'],'required');
$form->addRule('advisor_id',	WEBPAGE::$gt['selectOption'],'numeric');
$form->addRule('zone_id',		WEBPAGE::$gt['selectOption'],'required');
$form->addRule('zone_id',		WEBPAGE::$gt['selectOption'],'numeric');

//rules added for 2006-11-27 update
$form->addRule('email',          'myemail@mydomain.com','email');
$form->addRule('phone',          WEBPAGE::$conf['app']['regex_phone_tip'],'regex',WEBPAGE::$conf['app']['regex_phone']);
$form->addRule('mobile',         WEBPAGE::$conf['app']['regex_mobile_tip'],'regex',WEBPAGE::$conf['app']['regex_mobile']);

$form->freeze($client->frostdata);

if ($form->validate()) {

  $form->freeze();

  $values = $form->_submitValues;

  $values['editor_date']  = date('Y-m-d');
  $values['creator_date'] = $client->data['creator_date'] ? $client->data['creator_date'] : $values['editor_date'];
  $values['editor_id']    = WEBPAGE::$userID;
  $values['creator_id']   = $client->data['creator_id']   ? $client->data['creator_id']   : $values['editor_id'];

  $lastID = SQL::insert('tblClients',$values,'id');

  if ((!($id))||($values[activate])) {
    $iom = array(	'id'   			=> 'Null',
  					'type' 			=> 'I',
  					'client_id' 	=> $lastID,
  					'society_id' 	=> $values['society_id'],
			  		'advisor_id' 	=> $values['advisor_id'],
			  		'zone_id' 		=> $values['zone_id'],
			  		'date'	 		=> $values['editor_date'],
			  		'cause'			=> '',
			  		'user_id' 		=> WEBPAGE::$userID);

    SQL::insert('tblClientIOM',$iom);
    }

  WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewClient&id=%s&ico=scs&msg=scs',$lastID));

  }

$form->setDefaults($client->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

function checkcode($code) {

  $check = WEBPAGE::$dbh->getAll(sprintf("SELECT id, code FROM tblClients WHERE code = '%s' AND id != '%s'",$code,$_REQUEST['id']));
  if (count($check)) { return false; } else { return true; }

  }
?>