<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/coolbutton.php';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';
require_once 'class/society.php';

// Check for active loans. If so then abort moving...
/*
 * este approach evita movimiento con al menos un asociado dentro
 * del grupo origen tiene un prestamo activo o en proceso
$checkloan = WEBPAGE::$dbh->getAll(sprintf("select lm.id from tblLoansMaster as lm, tblClients as c where lm.borrower_id = IF(c.society_id,c.society_id,c.id) and c.id = %s and lm.check_status != 'R'",$_REQUEST['id']));
!count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewClient&id=%s&ico=err&msg=moveClient.err.loan',$_REQUEST['id']));
$checkloan = WEBPAGE::$dbh->getAll(sprintf("select lm.id, lcd.loan_id from tblLoansMaster as lm, tblLoansMasterDetails as lmd, tblClients as c, tblLoansCurrentData as lcd where lm.id = lmd.master_id and lmd.loan_id = lcd.loan_id and lm.borrower_id = IF(c.society_id,c.society_id,c.id) and c.id = %s and lm.check_status = 'R'",$_REQUEST['id']));
!count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewClient&id=%s&ico=err&msg=moveClient.err.loan',$_REQUEST['id']));
*/
// este approach solo verifica si el asociado en particular
// tiene prestamos activos o en proceso
$checkloan = WEBPAGE::$dbh->getAll(sprintf("select lcd.loan_id from tblLoansCurrentData as lcd, tblClients as c, tblLoans as l where lcd.loan_id = l.id and c.id = %s and l.client_id = c.id",$_REQUEST['id']));
!count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewClient&id=%s&ico=err&msg=moveClient.err.loan',$_REQUEST['id']));
$checkloan = WEBPAGE::$dbh->getAll(sprintf("select lm.id from tblLoansMaster as lm, tblLoansMasterDetails as lmd, tblClients as c, tblLoans as l where lm.check_status != 'R' and lm.id = lmd.master_id and lmd.loan_id = l.id and c.id = %s and l.client_id = c.id",$_REQUEST['id']));
!count($checkloan) 	? '' : WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewClient&id=%s&ico=err&msg=moveClient.err.loan',$_REQUEST['id']));


$client = new CLIENT($_REQUEST['id']);

if (!( $client->data[id] && $client->data[advisor_id] && $client->data[zone_id] )) {
  WEBPAGE::redirect("index.php?logout=1");
  }

$client->frostdata 			= array('nick','first','middle','last','birthdate','code','spouse','dependants','skills','education','cstatus','gender');

$btn  = new TTFButton(WEBPAGE::$theme);
$form = new HTML_QuickForm('clientForm','POST');

$button[id]	    		= "pickSocietybt";
$button[href]			= 'javascript:setSociety()';
$button[alt]			= WEBPAGE::$gt['pickSociety'];
$button[onClick]		= '';
$button[ico]			= "filter";
$picksocietybutton  	= New COOLBUTTON($button);

$form->addElement('static', 	'pickSociety', 	'', COOLBUTTON::getButton($picksocietybutton));

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
$form->addElement('text',		'society',		WEBPAGE::$gt['tblClients.society_id'],	'disabled class=large');
$form->addElement('select',		'education',	WEBPAGE::$gt['tblClients.education'],	(array('-'=>'') + $client->education()),	'class=large');
$form->addElement('select',		'cstatus',		WEBPAGE::$gt['tblClients.cstatus'],		(array('-'=>'') + $client->cstatus()),		'class=large');
$form->addElement('select',		'zone_id',		WEBPAGE::$gt['tblClients.zone_id'],		(array('-'=>'') + $client->zones()),		'class=large onChange="javascript:unsetSociety()"');
$form->addElement('select',		'advisor_id',	WEBPAGE::$gt['tblClients.advisor_id'],	(array('-'=>'') + $client->advisors()),		'class=large onChange="javascript:unsetSociety()"');

$form->addElement('textarea',	'memo',			WEBPAGE::$gt['tblClients.memo'], 		'class=large');

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

$form->freeze($client->frostdata);

if ($form->validate()) {

  $form->removeElement('pickSociety');
  $form->removeElement('submit');
  $form->removeElement('reset');
  $form->removeElement('cancel');

  $form->freeze();

  $values['id'] 			= $form->_submitValues[id];
  $values['advisor_id'] 	= $form->_submitValues[advisor_id];
  $values['zone_id'] 		= $form->_submitValues[zone_id];
  $values['society_id'] 	= $form->_submitValues[society_id];
  $values['editor_date']  	= date('Y-m-d');
  $values['editor_id']    	= WEBPAGE::$userID;

  $lastID = SQL::insert('tblClients',$values,'id');

  // record iom info
  $fields = array('id','internal','type','client_id','society_id','advisor_id','zone_id','date','user_id','memo');
  $data   = sprintf("
                    ('Null','1','O','%s','%s','%s','%s','%s','%s','%s'),
                    ('Null','1','I','%s','%s','%s','%s','%s','%s','%s')",
                     $client->data[id],$client->data[society_id],$client->data[advisor_id],$client->data[zone_id],date('Y-m-d'),WEBPAGE::$userID,$form->_submitValues['memo'],
                     $client->data[id],$values[society_id],$values[advisor_id],$values[zone_id],date('Y-m-d'),WEBPAGE::$userID,$form->_submitValues['memo']);

  SQL::insert_mult('tblClientIOM',$fields,$data);

  WEBPAGE::redirect(sprintf("index.php?scr_name=%s&id=%s",'BS.SCR.viewClient',$lastID));

  }

$form->setDefaults($client->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>