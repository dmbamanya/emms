<?php
$date = $_REQUEST['date'] ? $_REQUEST['date'] : date('Y-m-d');

$loan = WEBPAGE::$dbh->getAssoc(sprintf("
          select
            x.advisor_id, count(x.client_id) active
          from
            (
            select
              distinct l.client_id,l.advisor_id
            from
              view_loan_status_dates lsd,tblLoans l
            where
              lsd.active < '%s' and
              coalesce(lsd.cancelled,lsd.legal_in,'9999-01-01') >= '%s' and
              l.id = lsd.id
            ) x
          group by
            x.advisor_id", $date,$date));

$client = WEBPAGE::$dbh->getAll(sprintf("
            select
              u.id,
              concat(u.first,' ',u.last) advisor,
              sum(cp.clients)            clients,
              sum(cp.female)             female,
              sum(cp.male)               male,
              sum(cp.client_i)           client_i,
              sum(cp.client_g)           client_g,
              sum(cp.client_b)           client_b,
              sum(cp.group_g)            group_g,
              sum(cp.group_b)            group_b,
              sum(cp.group_bg)           group_bg
            from
              tblClientPortfolio cp,
              tblUsers           u
            where
              u.id    = cp.advisor_id and
              cp.date = '%s'
            group by
              cp.advisor_id", $date));

foreach ($client as $key => $value)
{
  $data[$value['id']]['advisor']  = $value['advisor'];
  $data[$value['id']]['clients']  = $value['clients'];
  $data[$value['id']]['loans']    = intval($loan[$value['id']]);
  $data[$value['id']]['female']   = $value['female'];
  $data[$value['id']]['male']     = $value['male'];
  $data[$value['id']]['client_i'] = $value['client_i'];
  $data[$value['id']]['client_g'] = $value['client_g'];
  $data[$value['id']]['client_b'] = $value['client_b'];
  $data[$value['id']]['group_g']  = $value['group_g'];
  $data[$value['id']]['group_b']  = $value['group_b'];
  $data[$value['id']]['group_bg'] = $value['group_bg'];
}

$total = current(WEBPAGE::$dbh->getAll(sprintf("
           select
             '%s'              advisor,
             sum(cp.clients)   clients,
             '%s'              loans,
             sum(cp.female)    female,
             sum(cp.male)      male,
             sum(cp.client_i)  client_i,
             sum(cp.client_g)  client_g,
             sum(cp.client_b)  client_b,
             sum(cp.group_g)   group_g,
             sum(cp.group_b)   group_b,
             sum(cp.group_bg)  group_bg
           from
             tblClientPortfolio cp,
             tblUsers           u
           where
             u.id    = cp.advisor_id and
             cp.date = '%s'", WEBPAGE::$gt['total'],array_sum($loan),$date)));

if (count($data)) { $data['total'] = $total; }

$head = array
        (
          'advisor'   => WEBPAGE::$gt['tblClients.advisor_id'],
          'clients'   => WEBPAGE::$gt['clients'],
          'loans'     => WEBPAGE::$gt['activeWithLoan'],
          'female'    => WEBPAGE::$gt['female'],
          'male'      => WEBPAGE::$gt['male'],
          'client_i'  => WEBPAGE::$gt['client_i'],
          'client_g'  => WEBPAGE::$gt['client_g'],
          'client_b'  => WEBPAGE::$gt['client_b'],
          'group_g'   => WEBPAGE::$gt['group_g'],
          'group_b'   => WEBPAGE::$gt['group_b'],
          'group_bg'  => WEBPAGE::$gt['group_bg']
        );

$tpl->setVariable('rpt_label',          WEBPAGE::$gt['clientPortfolio']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.clientByAdvisor']);
$tpl->setVariable('data_date_label',    WEBPAGE::$gt['date']);
$tpl->setVariable('data_date',          $date);
$tpl->setVariable('chart',              count($data) ? WEBPAGE::printchart($data,$head) : WEBPAGE::$gt['noData']);
?>
