<?php
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';

extract(CLIENT::getTemplateData($_REQUEST['id']));
$credit_data = CLIENT::getCreditHistory($_REQUEST['id']);

$btn  = new TTFButton(WEBPAGE::$theme);

$tpl->setVariable('name',               $name);
if ($society_id)
{
  $tpl->setVariable('society_label',    WEBPAGE::$gt['tblClients.society_id']);
  $tpl->setVariable('society',          $society);
  $tpl->setVariable('society_code',     $society_code);
}

$status = $advisor_id ? 'A' : 'I';

$tpl->setVariable('status_label',       WEBPAGE::$gt['tblClients.status']);
$tpl->setVariable('status',             WEBPAGE::$gt[sprintf('tblClients.status.%s',$status)]);
$tpl->setVariable('code_label',         WEBPAGE::$gt['tblClients.code']);
$tpl->setVariable('code',               $code);
$tpl->setVariable('skills_label',       WEBPAGE::$gt['tblClients.skills']);
$tpl->setVariable('skills',             $skills);
$tpl->setVariable('education_label',    WEBPAGE::$gt['tblClients.education']);
$tpl->setVariable('education',          $education);
$tpl->setVariable('dependants_label',   WEBPAGE::$gt['tblClients.dependants']);
$tpl->setVariable('dependants',         $dependants);
$tpl->setVariable('cstatus_label',      WEBPAGE::$gt['tblClients.cstatus']);
$tpl->setVariable('cstatus',            $cstatus);
$tpl->setVariable('spouse_label',       WEBPAGE::$gt['tblClients.spouse']);
$tpl->setVariable('spouse',             $spouse);
$tpl->setVariable('nick_label',         WEBPAGE::$gt['tblClients.nick']);
$tpl->setVariable('nick',               $nick);
$tpl->setVariable('advisor_label',      WEBPAGE::$gt['tblClients.advisor_id']);
$tpl->setVariable('advisor',            $advisor);
$tpl->setVariable('zone_label',         WEBPAGE::$gt['tblClients.zone_id']);
$tpl->setVariable('zone',               $zone);
$tpl->setVariable('birthdate_label',    WEBPAGE::$gt['tblClients.birthdate']);
$tpl->setVariable('birthdate',          $f_birthdate);
$tpl->setVariable('gender_label',       WEBPAGE::$gt['tblClients.gender']);
$tpl->setVariable('gender',             WEBPAGE::$gt[sprintf('tblUsers.gender.%s',$gender)]);
if (count($business))
{
  $tpl->setVariable('business',         $xpress ? implode('<br />',$business) : implode('<br />',WEBPAGE::hotlist('BS.SCR.viewBusiness','id',$business)));
  $tpl->setVariable('business_label',   WEBPAGE::$gt['tblBusiness.id']);
}
$tpl->setVariable('phone_label',         WEBPAGE::$gt['tblClients.phone']);
$tpl->setVariable('phone',               $phone);
$tpl->setVariable('mobile_label',        WEBPAGE::$gt['tblClients.mobile']);
$tpl->setVariable('mobile',              $mobile);
$tpl->setVariable('email_label',         WEBPAGE::$gt['tblClients.email']);
$tpl->setVariable('email',               $email);
$tpl->setVariable('address_label',       WEBPAGE::$gt['tblClients.address']);
$tpl->setVariable('address',             $address);
$tpl->setVariable('memo_label',          WEBPAGE::$gt['tblClients.memo']);
$tpl->setVariable('memo',                $memo);
$tpl->setVariable('creator_label',       WEBPAGE::$gt['tblClients.creator_id']);
$tpl->setVariable('creator',             sprintf(WEBPAGE::$gt['createdByOn'],$creator,$f_creator_date));
$tpl->setVariable('editor_label',        WEBPAGE::$gt['tblClients.editor_id']);
$tpl->setVariable('editor',              sprintf(WEBPAGE::$gt['editedByOn'],$editor,$f_editor_date));
$tpl->setVariable('img',                 $img_path);

/*
 * Create credit history chart
 */
$head = array
        (
          'loan_code'         => WEBPAGE::$gt['tblLoans.loan_code'],
          'status'            => WEBPAGE::$gt['status'],
          'delivered_date'    => WEBPAGE::$gt['tblLoans.delivered_date'],
          'cancel_date'       => WEBPAGE::$gt['cancel_date'],
          'kp'                => WEBPAGE::$gt['principal'],
          'balance_kp'        => WEBPAGE::$gt['tblPayments.balance_kp'],
          'pmt'               => WEBPAGE::$gt['tblPayments.id'],
          'xp_num_pmt'        => WEBPAGE::$gt['tblLoans.xp_num_pmt'],
          'payment_frequency' => WEBPAGE::$gt['tblLoanTypes.payment_frequency']
        );

foreach($credit_data as $key=>$row)
{
  $data[$key]['loan_code']         = $row['loan_code'];
  $data[$key]['status']            = WEBPAGE::$gt[sprintf('tblLoans.status.%s',$row['status'])];
  $data[$key]['delivered_date']    = $row['delivered_date'];
  $data[$key]['cancel_date']       = $row['cancel_date'];
  $data[$key]['kp']                = $row['kp'];
  $data[$key]['balance_kp']        = $row['balance_kp'];
  $data[$key]['pmt']               = sprintf(WEBPAGE::_HOTLINK_POPUP,WEBPAGE::$gt['tips']['view_pmt_plan'],'LN.SCR.pmtPlan','id',$row['id'],'LN.SCR.pmtPlan','no','yes','no','780','400',$row['pmt']);
  $data[$key]['xp_num_pmt']        = sprintf(WEBPAGE::_HOTLINK_POPUP,WEBPAGE::$gt['tips']['view_pmt_plan'],'LN.SCR.pmtPlan','id',$row['id'],'LN.SCR.pmtPlan','no','yes','no','780','400',$row['xp_num_pmt']);
  $data[$key]['payment_frequency'] = WEBPAGE::$gt[sprintf('tblLoanTypes.payment_frequency.%s',$row['payment_frequency'])];
}
if (count($credit_data) > 0)
{
  $tpl->setVariable('chart', WEBPAGE::printchart($data,$head));
  $tpl->setVariable('chart_label', WEBPAGE::$gt['credits']);
}

include './includes/ST.SCR.buttonMan.inc';

?>