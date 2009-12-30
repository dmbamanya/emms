<?php
$date_from = $_REQUEST['date_from'] ? $_REQUEST['date_from'] : date('Y-m-d');
$date_to   = $_REQUEST['date_to']   ? $_REQUEST['date_to']   : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
	  select
            l.advisor_id,
            concat(u.first,' ',u.last) advisor,
            count(l.id) loans,
            count(vl.id) nloans,
            sum(l.kp) kp,
            lt.borrower_type type
          from
          (
            tblLoans l,
            tblUsers u,
            tblLoanTypes lt
          )
          left join
            view_loan_1st_time vl on vl.id = l.id
          where
            u.id = l.advisor_id      and
            lt.id = l.loan_type_id   and
            l.delivered_date >= '%s' and
            l.delivered_date <= '%s'
          group by
            l.advisor_id,
            type", $date_from, $date_to));

foreach($data as $key=>$val)
{
  $ldata[$val['advisor_id']]['advisor']  = $val['advisor'];
  $ldata[$val['advisor_id']]['loans']   += $val['loans'];
  $ldata[$val['advisor_id']]['nloans']  += $val['nloans'];
  $ldata[$val['advisor_id']]['kp']      += $val['kp'];
  $ldata[$val['advisor_id']]['loans_I'] += ($val['type'] == 'I') ? $val['loans'] : 0;
  $ldata[$val['advisor_id']]['kp_I']    += ($val['type'] == 'I') ? $val['kp']    : 0;
  $ldata[$val['advisor_id']]['loans_G'] += ($val['type'] == 'G') ? $val['loans'] : 0;
  $ldata[$val['advisor_id']]['kp_G']    += ($val['type'] == 'G') ? $val['kp']    : 0;
  $ldata[$val['advisor_id']]['loans_B'] += ($val['type'] == 'B') ? $val['loans'] : 0;
  $ldata[$val['advisor_id']]['kp_B']    += ($val['type'] == 'B') ? $val['kp']    : 0;

  $total['advisor']                      = WEBPAGE::$gt['total'];
  $total['loans']                       += $val['loans'];
  $total['nloans']                      += $val['nloans'];
  $total['kp']                          += $val['kp'];
  $total['loans_I']                     += ($val['type'] == 'I') ? $val['loans'] : 0;
  $total['kp_I']                        += ($val['type'] == 'I') ? $val['kp']    : 0;
  $total['loans_G']                     += ($val['type'] == 'G') ? $val['loans'] : 0;
  $total['kp_G']                        += ($val['type'] == 'G') ? $val['kp']    : 0;
  $total['loans_B']                     += ($val['type'] == 'B') ? $val['loans'] : 0;
  $total['kp_B']                        += ($val['type'] == 'B') ? $val['kp']    : 0;
  }

if (count($ldata))
{
  $ldata['total'] = $total;
}

$head = array
        (
          'advisor' => WEBPAGE::$gt['tblClients.advisor_id'],
          'loans'   => sprintf('%s *',WEBPAGE::$gt['loans']),
          'nloans'  => sprintf('%s *',WEBPAGE::$gt['1sttimeloans']),
          'kp'      => sprintf('%s *',WEBPAGE::$gt['tblLoans.kp']),
          'loans_I' => sprintf('%s *',WEBPAGE::$gt['loans_I']),
          'kp_I'    => sprintf('%s *',WEBPAGE::$gt['kp_I']),
          'loans_G' => sprintf('%s *',WEBPAGE::$gt['loans_G']),
          'kp_G'    => sprintf('%s *',WEBPAGE::$gt['kp_G']),
          'loans_B' => sprintf('%s *',WEBPAGE::$gt['loans_B']),
          'kp_B'    => sprintf('%s *',WEBPAGE::$gt['kp_B'])
        );

$tpl->setVariable('rpt_label',          WEBPAGE::$gt['loanPortfolio']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.loanByAdvisor']);
$tpl->setVariable('date_range_label',   WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range',         sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart',              count($ldata) ? WEBPAGE::printchart($ldata,$head) : WEBPAGE::$gt['noData']);

$tpl->setVariable('releasedInPeriod',   WEBPAGE::$gt['releasedInPeriod']);

?>