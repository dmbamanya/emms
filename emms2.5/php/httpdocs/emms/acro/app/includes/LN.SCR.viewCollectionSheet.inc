<?php
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';
require_once 'class/society.php';

$client = new CLIENT();
extract(SOCIETY::getTemplateData($_REQUEST['id']));

$tpl->setVariable('viewCollectionSheet', WEBPAGE::$gt['LN.SCR.viewCollectionSheet']);
$tpl->setVariable('cn_date',             date('Y-m-d'));
$tpl->setVariable('society_label',       WEBPAGE::$gt[sprintf('tblSocieties.category.%s',$category)]);
$tpl->setVariable('society',             $name);
$tpl->setVariable('code',                $code);
$tpl->setVariable('zone_label',          WEBPAGE::$gt['tblSocieties.zone_id']);
$tpl->setVariable('zone',                $zone);

$mrow = WEBPAGE::$dbh->getAll(sprintf("
          select
            lmd.loan_id
          from
            tblLoansMasterDetails lmd
          where
            lmd.master_id in
              (
              select
                lm.id
              from
                tblLoansMaster lm
              where
                lm.borrower_type = 'B' and
                lm.borrower_id   = %s  and
                lm.check_status  = 'R'
              )", $_GET['id']));

foreach ($mrow as $key=>$val)
{
  $loans[] = $val['loan_id'];
}
$loans = implode(',',$loans);

$mrow = WEBPAGE::$dbh->getAll(sprintf("
          select
            l.client_id,
            concat(c.first,' ',c.last) client,
            l.loan_code,
            l.kp,
            l.kaf,
            l.kat,
            l.savings_p,
            l.savings_v,
            lcd.*
          from
            tblLoansCurrentData lcd,
            tblLoans l,
            tblClients c
          where
            c.id = l.client_id and
            l.id = lcd.loan_id and
            lcd.loan_id in (%s)", $loans));
$c = 1;
$data = array();
foreach($mrow as $key => $row)
{
  $client->data['id'] = $row['client_id'];
  $client->checkConfidentiality();
  if ($client->data[confidential])
  {
    $mrow[$key]['client'] = $client->data['name'];
  }
  $data[$key]['inx']         = $c++;
  $data[$key]['client']      = $mrow[$key]['client'];
  $data[$key]['kp']          = $mrow[$key]['kp'];
  $data[$key]['loan_code']   = $mrow[$key]['loan_code'];
  $data[$key]['balance_kp']  = $mrow[$key]['balance_kp'];
  $data[$key]['balance_kaf'] = $mrow[$key]['balance_kaf'];
  $data[$key]['balance_kat'] = $mrow[$key]['balance_kat'];
  $data[$key]['xp_pmt_date'] = $mrow[$key]['xp_pmt_date'];
  $data[$key]['xp_pmt']      = $mrow[$key]['xp_pmt'];
  $data[$key]['savings_p']   = $mrow[$key]['savings_p'];
  $data[$key]['savings_v']   = $mrow[$key]['savings_v'];
  $data[$key]['cn_delay']    = $mrow[$key]['cn_delay'];

  $total['inx']              = '';
  $total['client']           = WEBPAGE::$gt['total'];
  $total['kp']              += $mrow[$key]['kp'];
  $total['loan_code']        = '';
  $total['balance_kp']      += $mrow[$key]['balance_kp'];
  $total['balance_kaf']     += $mrow[$key]['balance_kaf'];
  $total['balance_kat']     += $mrow[$key]['balance_kat'];
  $total['xp_pmt_date']      = '';
  $total['xp_pmt']          += $mrow[$key]['xp_pmt'];
  $total['savings_p']       += $mrow[$key]['savings_p'];
  $total['savings_v']       += $mrow[$key]['savings_v'];
  $total['cn_delay']        += $mrow[$key]['cn_delay'];
}

if (count($data)) { $data['total'] = $total; }

$head = array
        (
           'inx'         => '#',
           'client'      => WEBPAGE::$gt['tblClients.id'],
           'kp'          => WEBPAGE::$gt['tblLoans.id'],
           'loan_code'   => WEBPAGE::$gt['tblLoans.loan_code'],
           'balance_kp'  => WEBPAGE::$gt['tblPayments.principal'].'*',
           'balance_kaf' => WEBPAGE::$gt['tblPayments.insurances'].'*',
           'balance_kat' => WEBPAGE::$gt['tblPayments.fees'].'*',
           'xp_pmt_date' => WEBPAGE::$gt['tblPayments.date'],
           'xp_pmt'      => WEBPAGE::$gt['tblPayments.pmt'],
           'savings_p'   => 'SAV-P',
           'savings_v'   => 'SAV-V',
           'cn_delay'    => WEBPAGE::$gt['tblPayments.delay'].'**'
         );

$tpl->setVariable('total_label',           WEBPAGE::$gt['total']);
$tpl->setVariable('pmt_label',             WEBPAGE::$gt['tblPayments.pmt']);
$tpl->setVariable('pmt_SAV_P_label',       sprintf('%s + %s',WEBPAGE::$gt['tblPayments.pmt'], WEBPAGE::$gt['tblLoans.savings_p']));
$tpl->setVariable('pmt_SAV_P_SAV_V_label', sprintf('%s + %s + %s',WEBPAGE::$gt['tblPayments.pmt'], WEBPAGE::$gt['tblLoans.savings_p'], WEBPAGE::$gt['tblLoans.savings_v']));
$tpl->setVariable('pmt',                   $total['xp_pmt']);
$tpl->setVariable('pmt_SAV_P',             $total['xp_pmt'] + $total['savings_p']);
$tpl->setVariable('pmt_SAV_P_SAV_V',       $total['xp_pmt'] + $total['savings_p'] + $total['savings_v']);
$tpl->setVariable('details',               WEBPAGE::$gt['details']);
$tpl->setVariable('chart',                 count($data) ? WEBPAGE::printchart($data,$head) : WEBPAGE::$gt['err']);
$tpl->setVariable('previousBalance',       WEBPAGE::$gt['previousBalance']);
$tpl->setVariable('days',                  WEBPAGE::$gt['days']);
$tpl->setVariable('legend',                sprintf("%s: <b>SAV-P</b> - %s, <b>SAV-V</b> - %s",WEBPAGE::$gt['legend'], WEBPAGE::$gt['tblLoans.savings_p'],WEBPAGE::$gt['tblLoans.savings_v']));
$tpl->setVariable('print',                 WEBPAGE::$gt['printSheet']);
?>