<?php
$date_from = $_REQUEST['date_from'] ? $_REQUEST['date_from'] : date('Y-m-d');
$date_to   = $_REQUEST['date_to']   ? $_REQUEST['date_to']   : date('Y-m-d');

$data = WEBPAGE::$dbh->getAll(sprintf("
          select
            l.program_id      program_id,
            pg.program        program,
            lt.borrower_type  type,
            sum(p.pmt)        pmt,
            sum(p.principal)  principal,
            sum(pd.principal) principal_d,
            sum(p.insurances) insurances,
            sum(p.fees)       fees,
            sum(p.interest)   interest,
            sum(p.penalties)  penalties
          from
            (
            tblPayments  p,
            tblLoans     l,
            tblPrograms  pg,
            tblLoanTypes lt
            )
          left join
            tblPayments  pd on pd.id = p.id and pd.penalties > 0
          where
            l.id    = p.loan_id      and
            pg.id   = l.program_id      and
            lt.id   = l.loan_type_id and
            p.date >= '%s'           and
            p.date <= '%s'
          group by
            l.program_id, type", $date_from, $date_to));

$total = array();
foreach($data as $key=>$val)
{
  $ldata[$val['program_id']]['program']        = str_replace(' ','&nbsp;',$val['program']);

  $ldata[$val['program_id']]['pmt']           += $val['pmt'];
  $ldata[$val['program_id']]['principal']     += $val['principal'];
  $ldata[$val['program_id']]['principal_d']   += $val['principal_d'];
  $ldata[$val['program_id']]['insurances']    += $val['insurances'];
  $ldata[$val['program_id']]['fees']          += $val['fees'];
  $ldata[$val['program_id']]['interest']      += $val['interest'];
  $ldata[$val['program_id']]['penalties']     += $val['penalties'];

  $ldata[$val['program_id']]['pmt_I']         += ($val['type'] == 'I') ? $val['pmt']         : 0;
  $ldata[$val['program_id']]['principal_I']   += ($val['type'] == 'I') ? $val['principal']   : 0;
  $ldata[$val['program_id']]['principal_d_I'] += ($val['type'] == 'I') ? $val['principal_d'] : 0;
  $ldata[$val['program_id']]['insurances_I']  += ($val['type'] == 'I') ? $val['insurances']  : 0;
  $ldata[$val['program_id']]['fees_I']        += ($val['type'] == 'I') ? $val['fees']        : 0;
  $ldata[$val['program_id']]['interest_I']    += ($val['type'] == 'I') ? $val['interest']    : 0;
  $ldata[$val['program_id']]['penalties_I']   += ($val['type'] == 'I') ? $val['penalties']   : 0;

  /*
  $ldata[$val['program_id']]['pmt_G']         += ($val['type'] == 'G') ? $val['pmt']         : 0;
  $ldata[$val['program_id']]['principal_G']   += ($val['type'] == 'G') ? $val['principal']   : 0;
  $ldata[$val['program_id']]['principal_d_G'] += ($val['type'] == 'G') ? $val['principal_d'] : 0;
  $ldata[$val['program_id']]['insurances_G']  += ($val['type'] == 'G') ? $val['insurances']  : 0;
  $ldata[$val['program_id']]['fees_G']        += ($val['type'] == 'G') ? $val['fees']        : 0;
  $ldata[$val['program_id']]['interest_G']    += ($val['type'] == 'G') ? $val['interest']    : 0;
  $ldata[$val['program_id']]['penalties_G']   += ($val['type'] == 'G') ? $val['penalties']   : 0;
  */

  $ldata[$val['program_id']]['pmt_B']         += ($val['type'] == 'B') ? $val['pmt']         : 0;
  $ldata[$val['program_id']]['principal_B']   += ($val['type'] == 'B') ? $val['principal']   : 0;
  $ldata[$val['program_id']]['principal_d_B'] += ($val['type'] == 'B') ? $val['principal_d'] : 0;
  $ldata[$val['program_id']]['insurances_B']  += ($val['type'] == 'B') ? $val['insurances']  : 0;
  $ldata[$val['program_id']]['fees_B']        += ($val['type'] == 'B') ? $val['fees']        : 0;
  $ldata[$val['program_id']]['interest_B']    += ($val['type'] == 'B') ? $val['interest'] 	  : 0;
  $ldata[$val['program_id']]['penalties_B']   += ($val['type'] == 'B') ? $val['penalties']   : 0;

  $total['program']        =  WEBPAGE::$gt['total'];

  $total['pmt']           += $val['pmt'];
  $total['principal']     += $val['principal'];
  $total['principal_d']   += $val['principal_d'];
  $total['insurances']    += $val['insurances'];
  $total['fees']          += $val['fees'];
  $total['interest']      += $val['interest'];
  $total['penalties']     += $val['penalties'];

  $total['pmt_I']         += ($val['type'] == 'I') ? $val['pmt']         : 0;
  $total['principal_I']   += ($val['type'] == 'I') ? $val['principal']   : 0;
  $total['principal_d_I'] += ($val['type'] == 'I') ? $val['principal_d'] : 0;
  $total['insurances_I']  += ($val['type'] == 'I') ? $val['insurances']  : 0;
  $total['fees_I']        += ($val['type'] == 'I') ? $val['fees']        : 0;
  $total['interest_I']    += ($val['type'] == 'I') ? $val['interest'] 	 : 0;
  $total['penalties_I']   += ($val['type'] == 'I') ? $val['penalties'] 	 : 0;

  /*
  $total['pmt_G']         += ($val['type'] == 'G') ? $val['pmt']         : 0;
  $total['principal_G']   += ($val['type'] == 'G') ? $val['principal']   : 0;
  $total['principal_d_G'] += ($val['type'] == 'G') ? $val['principal_d'] : 0;
  $total['insurances_G']  += ($val['type'] == 'G') ? $val['insurances']  : 0;
  $total['fees_G']        += ($val['type'] == 'G') ? $val['fees']        : 0;
  $total['interest_G']    += ($val['type'] == 'G') ? $val['interest']    : 0;
  $total['penalties_G']   += ($val['type'] == 'G') ? $val['penalties'] 	 : 0;
  */

  $total['pmt_B']         += ($val['type'] == 'B') ? $val['pmt']         : 0;
  $total['principal_B']   += ($val['type'] == 'B') ? $val['principal'] 	 : 0;
  $total['principal_d_B'] += ($val['type'] == 'B') ? $val['principal_d'] : 0;
  $total['insurances_B']  += ($val['type'] == 'B') ? $val['insurances']  : 0;
  $total['fees_B']        += ($val['type'] == 'B') ? $val['fees']        : 0;
  $total['interest_B']    += ($val['type'] == 'B') ? $val['interest'] 	 : 0;
  $total['penalties_B']   += ($val['type'] == 'B') ? $val['penalties'] 	 : 0;
}

if (count($total)) { $ldata['total'] = $total; }

/*
 *  applies currency format to array data
 */
foreach($ldata as $key => $value)
{
  foreach ($value as $key_inner=>$value_inner)
  {
    if (is_float($value_inner))
    {
      $ldata[$key][$key_inner] = number_format($ldata[$key][$key_inner],2,'.',',');
    }
  }
}

$head = array
        (
          'program'        => WEBPAGE::$gt['tblPrograms.program'],
          'pmt'            => sprintf('PMT-T*'),
          'principal'      => sprintf('KP-T*'),
          'principal_d'    => sprintf('KPD-T*'),
          'insurances'     => sprintf('INS-T*'),
          'fees'           => sprintf('FEES-T*'),
          'interest'       => sprintf('RT-T*'),
          'penalties'      => sprintf('PN-T*'),
          'pmt_I'          => sprintf('PMT-I*'),
          'principal_I'    => sprintf('KP-I*'),
          'principal_d_I'  => sprintf('KPD-I*'),
          'insurances_I'   => sprintf('INS-I*'),
          'fees_I'         => sprintf('FEES-I*'),
          'interest_I'     => sprintf('RT-I*'),
          'penalties_I'    => sprintf('PN-I*'),
          /*
          'pmt_G'          => sprintf('PMT-G*'),
          'principal_G'    => sprintf('KP-G*'),
          'principal_d_G'  => sprintf('KPD-G*'),
          'insurances_G'   => sprintf('INS-G*'),
          'fees_G'         => sprintf('FEES-G*'),
          'interest_G'     => sprintf('RT-G*'),
          'penalties_G'    => sprintf('PN-G*'),
           */
          'pmt_B'          => sprintf('PMT-B*'),
          'principal_B'    => sprintf('KP-B*'),
          'principal_d_B'  => sprintf('KPD-B*'),
          'insurances_B'   => sprintf('INS-B*'),
          'fees_B'         => sprintf('FEES-B*'),
          'interest_B'     => sprintf('RT-B*'),
          'penalties_B'    => sprintf('PN-B*')
        );

$tpl->setVariable('rpt_label',          WEBPAGE::$gt['recovery']);
$tpl->setVariable('rpt_subtitle_label', WEBPAGE::$gt['RP.SCR.recoveryByProgram']);
$tpl->setVariable('date_range_label', 	WEBPAGE::$gt['dateRange']);
$tpl->setVariable('date_range',         sprintf('%s : %s',$date_from,$date_to));

$tpl->setVariable('chart', count($ldata) ? WEBPAGE::printchart($ldata,$head) : WEBPAGE::$gt['noData']);

$tpl->setVariable('recoveryInPeriod', 	WEBPAGE::$gt['recoveryInPeriod']);

$tpl->setVariable('legend', WEBPAGE::$gt['legend']);
$tpl->setVariable('PMT',    WEBPAGE::$gt['PMT']);
$tpl->setVariable('KP',     WEBPAGE::$gt['KP']);
$tpl->setVariable('KPD',    WEBPAGE::$gt['KPD']);
$tpl->setVariable('INS',    WEBPAGE::$gt['INS']);
$tpl->setVariable('FEES',   WEBPAGE::$gt['FEES']);
$tpl->setVariable('RT',     WEBPAGE::$gt['RT']);
$tpl->setVariable('PN',     WEBPAGE::$gt['PN']);
$tpl->setVariable('T',      WEBPAGE::$gt['R-T']);
$tpl->setVariable('I',      WEBPAGE::$gt['R-I']);
$tpl->setVariable('B',      WEBPAGE::$gt['R-B']);
?>