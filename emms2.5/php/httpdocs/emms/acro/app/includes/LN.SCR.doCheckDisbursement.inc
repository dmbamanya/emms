<?php;
/*
 * being defensive against hacking... only loan master with all loans in status = 'A' can be disbursed
 */
if (current(current(WEBPAGE::$dbh->getAll(sprintf("select count(l.id) from tblLoans l, tblLoansMasterDetails lmd where l.status != 'A' and lmd.master_id = %s and lmd.loan_id = l.id",$_REQUEST['id'])))))
{
  WEBPAGE::redirect("index.popup.php?scr_name=exit");
  exit;
}

require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/loan_master.php';
require_once 'class/program.php';
require_once '../mod_sponsorship/class/sponsor.php';

$master 	= new LOAN_MASTER($_REQUEST['id']);   
$program 	= new PROGRAM($master->data['program_id']);
$sponsor 	= new SPONSOR();
$btn  		= new TTFButton(WEBPAGE::$theme);
$form 		= new HTML_QuickForm('disbursementForm','POST');

$form->addElement('static',   'submit',   '', $btn->imgsubmit('disbursementForm',WEBPAGE::$gt['submit']));
$form->addElement('static',   'reset',    '', $btn->imgreset('disbursementForm',WEBPAGE::$gt['reset']));
$form->addElement('static',   'cancel',   '', $btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden',   'scr_name',     $_REQUEST['scr_name']);
$form->addElement('hidden',   'id',           $_REQUEST['id']);
$form->addElement('text',     'check_number', WEBPAGE::$gt['tblLoansMaster.check_number'], 'class=large');
$form->addElement('select',   'fund',         WEBPAGE::$gt['tblFunds.id'],         (array('-'=>'') + $program->getFunds()), 'class=large');

$sponsorsByBalance = $sponsor->getSponsorsByBalance($master->data['amount'], 1);
$sponsorsByZone    = $sponsor->getSponsorsByZone($master->getBorrowerZone());
$sponsors          = array_intersect_key($sponsorsByBalance,$sponsorsByZone);
arsort($sponsors);
foreach($sponsors as $key => $val) $sponsors[$key] = $sponsorsByZone[$key];
$sponsors = WEBPAGE::$conf['mod_sponsorship']['disbursement_filter'] ? array_slice($sponsors, 0, 1) : $sponsors;
$form->addElement('select',   'sponsor_id',   WEBPAGE::$gt['tblSponsors.sponsor'], (array(''=>'') + $sponsors), 'class=large');

$form->addElement('textarea', 'memo',         WEBPAGE::$gt['memo'], 'rows=5');

$form->addRule('check_number',   sprintf(WEBPAGE::$gt['rangelength'],4,32),'required');
$form->registerRule('checkcode', 'callback', 'checkcode');
$form->addRule('check_number', 	 sprintf(WEBPAGE::$gt['rangelength'],4,32), 'checkcode', true);

$form->addRule('fund',         WEBPAGE::$gt['selectOption'],'required');
$form->addRule('fund',         WEBPAGE::$gt['selectOption'],'alphanumeric');
$form->addRule('memo',         sprintf(WEBPAGE::$gt['maxlength'],255),'maxlength', 255);

if ($form->validate())
{   
  $form->freeze();  
  $values = $form->_submitValues;
  $new_loan_status        = 'D'; //Disbursed
  $new_loan_master_status = 'D'; //Disbursed
	
  /*
   * Insert new record at tblFundsLoansMasterPct
   */
  SQL::insert('tblFundsLoansMasterPct',array('id'=>'Null','master_id'=>$values['id'],'fund_id'=>$values['fund'],'pct'=>'100'));
  /*
   * Insert loan status log
   */
  WEBPAGE::$dbh->query(sprintf("insert into tblLoanStatusHistory (loan_id,p_status,status,date,user_id,memo) select l.id,l.status,'%s','%s','%s','%s' from tblLoans as l, tblLoansMasterDetails as lmd where lmd.master_id = %s AND lmd.loan_id = l.id",$new_loan_status,date('Y-m-d'),WEBPAGE::$userID,$values['memo'],$values['id']));
  /*
   * update loans status
   */
  WEBPAGE::$dbh->query(sprintf("update tblLoans as l set l.status = '%s' where l.id in (select lmd.loan_id from tblLoansMasterDetails as lmd where lmd.master_id = %s)",$new_loan_status,$values['id']));
  /*
   * update check status
   */
  WEBPAGE::$dbh->query(sprintf("update tblLoansMaster set check_status = '%s', check_number = '%s', sponsor_id = '%s' where id = %s",$new_loan_master_status,$values['check_number'],$values['sponsor_id'],$values['id']));

  /*
   * refresh parent window with a javascript call
   */
  $tpl->setVariable('refreshParent','refreshParent();');

} else {
 
  /*
   * print the form
   */
  $form->setDefaults();
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $form->accept($renderer);

  $tpl->setVariable('form_name', WEBPAGE::$gt['LN.SCR.doCheckDisbursement']);
  $tpl->setVariable('div_xls_download', 'remove');
  $tpl->setVariable('msg_label', WEBPAGE::$gt['important']);
  $tpl->setVariable('msg_text',  WEBPAGE::$gt['check_details_to_submit']);

  $tpl->setVariable('clickToRetractRequest', WEBPAGE::$gt['clickToRetractRequest']);
  $tpl->setVariable('master_id', $_REQUEST['id']);
  $tpl->setVariable('more_options', WEBPAGE::$gt['more_options']);
  require_once './includes/LN.SCR.viewLoanMasterPrintable.inc';
}

function checkcode($code)
{
  $c = explode(' ', $code);
  if (count($c) > 1) return false;
  return true;
}
?>

