<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/loan_type.php';

$ltype = new LOAN_TYPE();

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 'submit',    '', WEBPAGE::$btn->imgsubmit('searchForm',WEBPAGE::$gt['search']));
$form->addElement('static', 'reset',     '', WEBPAGE::$btn->imgreset('searchForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 'cancel',    '', WEBPAGE::$btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden', 'scr_name',      $_REQUEST['scr_name']);
$form->addElement('select', 'borrower_type', WEBPAGE::$gt['tblLoanTypes.borrower_type'], ($borrower_types = $ltype->borrower_types()));

if ($form->validate())
{ 
  $values = $form->_submitValues;

  $extra['param']['value']['borrower_type']              = $values['borrower_type'];
  $extra['param']['label']['tblLoanTypes.borrower_type'] = $borrower_types[$values['borrower_type']];

  $fields[]                       = 'description';
  $extra['description']['header'] = 'tblLoanTypes.description';
  $extra['description']['pack']   = '%s';
  $extra['description']['align']  = 'left';
    
  $fields[]                       = 'id as view';
  $extra['view']['header']        = 'view';
  $extra['view']['pack']          = sprintf("<a href='%s?scr_name=LN.SCR.viewLoanType&ref=LN.SCR.browseLoanTypes&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['view']);
  $extra['view']['align']         = 'center';

  $fields[]                       = 'id as edit';
  $extra['edit']['header']        = 'edit';
  $extra['edit']['pack']          = sprintf("<a href='%s?scr_name=LN.SCR.addLoanType&ref=LN.SCR.editLoanTypes&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['edit']);
  $extra['edit']['align']         = 'center';

  $extra['param']['ref']          = WEBPAGE::$scr_name;
  $extra['param']['ts']           = microtime(true);
  
  $mrow = SQL::select('tblLoanTypes',$fields,sprintf("borrower_type = '%s'",$values['borrower_type']));
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