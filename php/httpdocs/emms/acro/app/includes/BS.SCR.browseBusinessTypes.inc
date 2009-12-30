<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/business_type.php';

$btype = new BUSINESS_TYPE();

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 'submit', '', WEBPAGE::$btn->imgsubmit('searchForm',WEBPAGE::$gt['search']));
$form->addElement('static', 'reset',  '', WEBPAGE::$btn->imgreset('searchForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 'cancel', '', WEBPAGE::$btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden', 'scr_name',   $_REQUEST['scr_name']);
$form->addElement('select', 'activity',   WEBPAGE::$gt['tblBusinessTypes.activity'], ($activities = $btype->activities()));

if ($form->validate())
{ 
  $values = $form->_submitValues;

  $fields[]                = "type";
  $extra['type']['header'] = 'tblBusinessTypes.type';
  $extra['type']['pack']   = '%s';
  $extra['type']['align']  = 'left';

  $fields[]                = 'id as edit';
  $extra['edit']['header'] = 'edit';
  $extra['edit']['pack']   = sprintf("<a href='%s?scr_name=BS.SCR.addBusinessType&ref=BS.SCR.editBusinessType&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['edit']);
  $extra['edit']['align']  = 'center';

  $param = sprintf("activity = '%s'", $values['activity']);
  
  $extra['param']['value']['activity']                  = $values['activity'];
  $extra['param']['label']['tblBusinessTypes.activity']	= WEBPAGE::$gt[sprintf("tblBusinessTypes.activity.%s",$values['activity'])];

  $extra['param']['ref'] = WEBPAGE::$scr_name;
  $extra['param']['ts']  = microtime(true);
  
  $mrow = SQL::select('tblBusinessTypes',$fields,$param);
  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
  require_once 'BS.SCR.pager.inc';
  
} else {

  $form->setDefaults();
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $tpl->setVariable('content_title',WEBPAGE::$gt[WEBPAGE::$scr_name]);
  $form->accept($renderer);
}

?>