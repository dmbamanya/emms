<?php
$date_from = $_REQUEST['date_from'] ? $_REQUEST['date_from'] : date('Y-m-d');
$date_to   = $_REQUEST['date_to']   ? $_REQUEST['date_to']   : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
	  select
            l.zone_id,
            z.zone zone,
            count(l.id) loans,
            count(vl.id) nloans,
            sum(l.kp) kp,
            lt.borrower_type type
          from
          (
            tblLoans l,
            tblZones z,
            tblLoanTypes lt 
          )
          left join
            view_loan_1st_time vl on vl.id = l.id
          where
            z.id              = l.zone_id and
            lt.id             = l.loan_type_id and
            l.delivered_date >= '%s' and
            l.delivered_date <= '%s'
          group by
            l.zone_id,
            type", $date_from, $date_to));

$ldata = array();
$total = array();
foreach($data as $key=>$val)
{
  $ldata[$val['zone_id']]['zone']     = $val['zone'];
  $ldata[$val['zone_id']]['loans']   += $val['loans'];
  $ldata[$val['zone_id']]['nloans']  += $val['nloans'];
  $ldata[$val['zone_id']]['kp']      += $val['kp'];
  $ldata[$val['zone_id']]['loans_I'] += ($val['type'] == 'I') ? $val['loans'] : 0;
  $ldata[$val['zone_id']]['kp_I']    += ($val['type'] == 'I') ? $val['kp']    : 0;
  $ldata[$val['zone_id']]['loans_G'] += ($val['type'] == 'G') ? $val['loans'] : 0;
  $ldata[$val['zone_id']]['kp_G']    += ($val['type'] == 'G') ? $val['kp']    : 0;
  $ldata[$val['zone_id']]['loans_B'] += ($val['type'] == 'B') ? $val['loans'] : 0;
  $ldata[$val['zone_id']]['kp_B']    += ($val['type'] == 'B') ? $val['kp']    : 0;
  
  $total['zone']                      = WEBPAGE::$gt['total'];
  $total['loans']                    += $val['loans'];
  $total['nloans']                   += $val['nloans'];
  $total['kp']                       += $val['kp'];
  $total['loans_I']                  += ($val['type'] == 'I') ? $val['loans'] : 0;
  $total['kp_I']                     += ($val['type'] == 'I') ? $val['kp']    : 0;
  $total['loans_G']                  += ($val['type'] == 'G') ? $val['loans'] : 0;
  $total['kp_G']                     += ($val['type'] == 'G') ? $val['kp']    : 0;
  $total['loans_B']                  += ($val['type'] == 'B') ? $val['loans'] : 0;
  $total['kp_B']                     += ($val['type'] == 'B') ? $val['kp']    : 0;
}

if (count($ldata))
{
  $ldata['total'] = $total;
}

$head = array
        (
          'zone'    => WEBPAGE::$gt['tblZones.zone'],
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
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.loanByBranch']);
$tpl->setVariable('date_range_label',   WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range',         sprintf('%s : %s',$date_from,$date_to));
$tpl->setVariable('chart',              count($ldata) ? WEBPAGE::printchart($ldata,$head) : WEBPAGE::$gt['noData']);

$tpl->setVariable('releasedInPeriod',   WEBPAGE::$gt['releasedInPeriod']);

?>
