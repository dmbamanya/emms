<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/client.php';
require_once 'class/business_type.php';
require_once 'class/business.php';

$bz = new BUSINESS($_REQUEST['id']);
if ($_GET['owner_id']) {
  $cl = new CLIENT($_GET['owner_id']);
  $bz->data[owner1] = $cl->data['code'];
  $bz->frostdata = array('owner1','owner2','owner3','owner4','owner5');
  }
$btn  = new TTFButton(WEBPAGE::$theme);

$form = new HTML_QuickForm('bzForm','POST');

$form->addElement('static', 	'submit', 	'', $btn->imgsubmit('bzForm',WEBPAGE::$gt['submit']));
$form->addElement('static', 	'reset', 	'', $btn->imgreset('bzForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 	'cancel', 	'', $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));

$form->addElement('hidden',		'scr_name',		$_REQUEST['scr_name']);
$form->addElement('hidden',		'ref',			$_REQUEST['ref']);
$form->addElement('hidden',		'loan_id',		$_REQUEST['loan_id']);
$form->addElement('hidden',		'client_id',	$_REQUEST['owner_id']);
$form->addElement('hidden',		'id',			$_REQUEST['id']);

$form->addElement('text',		'name',			WEBPAGE::$gt['tblBusiness.name'], 		'class=large');
$form->addElement('select',		'type_id',		WEBPAGE::$gt['tblBusiness.type_id'],	(array('-'=>'') + $bz->types()), 'class=large');
$form->addElement('text',		'owner1',		WEBPAGE::$gt['tblBusiness.client_list'], 'class=large');
$form->addElement('text',		'owner2',		'', 'class=large');
$form->addElement('text',		'owner3',		'', 'class=large');
$form->addElement('text',		'owner4',		'', 'class=large');
$form->addElement('text',		'owner5',		'', 'class=large');

$form->addElement('textarea',	'description',	WEBPAGE::$gt['tblBusiness.description'], 'rows = 8');

$client_list = array();
function checkowner($code = '') {
  global $client_list;
  if (!($code)) { return true; }
  if ( $owner_id = current(SQL::getAssoc('tblClients','code,id',sprintf("code = '%s' AND advisor_id != 0",$code)))) {
    return ($client_list[] = $owner_id);
	} else {
    return false;
    }
  }
//add rules
$form->addRule('name',			sprintf(WEBPAGE::$gt['rangelength'],1,32),'required');
$form->addRule('name',			sprintf(WEBPAGE::$gt['rangelength'],1,32),'rangelength', array(1,32));
$form->addRule('owner1',		WEBPAGE::$conf['app']['regex_govID_tip'],'required');
$form->addRule('owner1',		WEBPAGE::$conf['app']['regex_govID_tip'],'regex',WEBPAGE::$conf['app']['regex_govID']);
$form->addRule('owner1',		WEBPAGE::$gt['nonValidEntry'],'callback','checkowner');
$form->addRule('owner2',		WEBPAGE::$gt['nonValidEntry'],'callback','checkowner');
$form->addRule('owner3',		WEBPAGE::$gt['nonValidEntry'],'callback','checkowner');
$form->addRule('owner4',		WEBPAGE::$gt['nonValidEntry'],'callback','checkowner');
$form->addRule('owner5',		WEBPAGE::$gt['nonValidEntry'],'callback','checkowner');
$form->addRule('name',			WEBPAGE::$gt['lettersonly'],'regex', '(^[a-zA-Z\. ]*$)');
$form->addRule('type_id',		WEBPAGE::$gt['selectOption'],'required');
$form->addRule('type_id',		WEBPAGE::$gt['selectOption'],'alphanumeric');
$form->addRule('description',	sprintf(WEBPAGE::$gt['maxlength'],255),'maxlength', 255);

$form->freeze($bz->frostdata);

if ($form->validate()) {

  $form->freeze();
  $values = $form->_submitValues;

  $values['client_list']	= implode(',',array_unique($client_list));
  $values['editor_date'] 	= date('Y-m-d');
  $values['creator_date'] 	= $bz->data['creator_date'] ? $bz->data['creator_date'] : $values['editor_date'];
  $values['editor_id']   	= WEBPAGE::$userID;
  $values['creator_id']  	= $bz->data['creator_id']   ? $bz->data['creator_id']   : $values['editor_id'];

  $lastID = SQL::insert('tblBusiness',$values,'id');

  if (!($values['id'])) {
    $iom['id'] = 'Null';
    $iom['business_id'] = $lastID;
    $iom['status'] = 'N';
    $iom['creator_date'] = $values['editor_date'];
    $iom['creator_id'] = WEBPAGE::$userID;
    SQL::insert('tblBusinessIOM',$iom);
    }

  if ($_REQUEST['loan_id']) {
    WEBPAGE::redirect(sprintf('index.php?scr_name=LN.SCR.pickBusiness&loan_id=%s&business_id=%s',$_REQUEST['loan_id'],$lastID));
    }

  WEBPAGE::redirect(sprintf('index.php?scr_name=BS.SCR.viewBusiness&id=%s&ico=scs&msg=scs',$lastID));

  }

$form->setDefaults($bz->data);
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$form->accept($renderer);

?>