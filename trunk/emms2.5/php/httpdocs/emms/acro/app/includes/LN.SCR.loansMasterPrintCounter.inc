<?php
/*
 * Increments tblLoansMaster.chk_process counter if check status is 'Approved'
 */
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';

$master = new LOAN_MASTER($_REQUEST['id']);

if ($master->data['check_status'] != 'A')
{
  $tpl->setVariable('hits','');
} else {
  WEBPAGE::$dbh->query(sprintf("update tblLoansMaster set chk_process = (chk_process + 1) where id = %s",$master->data['id']));
  $tpl->setVariable('hits',1);   
}
?>
