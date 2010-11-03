<?php
$date_from 	= $_REQUEST['date_from'] 	? $_REQUEST['date_from'] 	: date('Y-m-d');
$date_to 	= $_REQUEST['date_to'] 		? $_REQUEST['date_to'] 		: date('Y-m-d');
$tpl->setVariable('dateRangeSelector',  WEBPAGE::$gt['BS.SCR.dateRangeSelector']);
$tpl->setVariable('oldrange',           sprintf('%s - %s',$date_from,$date_to));

$datac = WEBPAGE::$dbh->getAll(sprintf("
           select
             l.id                        loan_id      ,
             l.loan_code                 loan_code    ,
             z.short_name                branch       ,
             ua.username                 loan_officer ,
             concat(c.first, ' ',c.last) client       ,
             c.code                      client_code  ,
             lwoc.amount                 charge       ,
             ''                          pmt          ,
             lwoc.date                   date         ,
             u.username                  registered_by
           from
             tblLoanWriteOffCharges  lwoc,
             tblLoans                l   ,
             tblClients              c   ,
             tblUsers                u   ,
             tblZones                z   ,
             tblUsers                ua
           where
             lwoc.date >= '%s' and
             lwoc.date <= '%s' and
             u.id       = lwoc.user_id and
             l.id       = lwoc.loan_id and
             c.id       = l.client_id  and
             z.id       = l.zone_id    and
             ua.id      = l.advisor_id", $date_from, $date_to));

$datap = WEBPAGE::$dbh->getAll(sprintf("
           select
             l.id                        loan_id      ,
             l.loan_code                 loan_code    ,
             z.short_name                branch       ,
             ua.username                 loan_officer ,
             concat(c.first, ' ',c.last) client       ,
             c.code                      client_code  ,
             ''                          charge       ,
             lwop.amount                 pmt          ,
             lwop.date                   date         ,
             u.username                  registered_by
           from
             tblLoanWriteOffPayments lwop,
             tblLoans                l   ,
             tblClients              c   ,
             tblUsers                u   ,
             tblZones                z   ,
             tblUsers                ua
           where
             lwop.date >= '%s' and
             lwop.date <= '%s' and
             u.id       = lwop.user_id and
             l.id       = lwop.loan_id and
             c.id       = l.client_id  and
             z.id       = l.zone_id    and
             ua.id      = l.advisor_id", $date_from, $date_to));

$data = array_sort($datac + $datap, 'date', 'SORT_ASC');

foreach($data as $key=>$val)
{
  $data[$key]['loan_code'] = sprintf('<a href=\'javascript:openWin("index.popup.php?scr_name=LG.SCR.legalProfile&id=%s","%s","menubar=no,scrollbars=yes,resizable=yes,width=700,height=320")\' onClick="">%s</a>',$val['loan_id'],$val['loan_code'],$val['loan_code']);
}

$head = array
        (
          'loan_code'     => WEBPAGE::$gt['tblLoans.loan_code'],
          'branch'        => WEBPAGE::$gt['tblZones.zone'],
          'loan_officer'  => WEBPAGE::$gt['tblLoans.advisor_id'],
          'client'        => WEBPAGE::$gt['tblClients.id'],
          'client_code'   => WEBPAGE::$gt['tblClients.code'],
          'charge'        => WEBPAGE::$gt['charge'],
          'pmt'           => WEBPAGE::$gt['payment'],
          'date'          => WEBPAGE::$gt['date'],
          'registered_by' => WEBPAGE::$gt['tblUsers.id']
         );

$tpl->setVariable('rpt_label',          WEBPAGE::$gt['transactionsLegals']);
$tpl->setVariable('date_range_label',   WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range',         sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart',              count($data) ? WEBPAGE::printchart($data,$head) : WEBPAGE::$gt['noData']);


   function array_sort($array, $on, $order='SORT_DESC')
    {
      $new_array = array();
      $sortable_array = array();

      if (count($array) > 0) {
          foreach ($array as $k => $v) {
              if (is_array($v)) {
                  foreach ($v as $k2 => $v2) {
                      if ($k2 == $on) {
                          $sortable_array[$k] = $v2;
                      }
                  }
              } else {
                  $sortable_array[$k] = $v;
              }
          }

          switch($order)
          {
              case 'SORT_ASC':
                  //echo "ASC";
                  asort($sortable_array);
              break;
              case 'SORT_DESC':
                  //echo "DESC";
                  arsort($sortable_array);
              break;
          }

          foreach($sortable_array as $k => $v) {
              $new_array[] = $array[$k];
          }
      }
      return $new_array;
    }
?>
