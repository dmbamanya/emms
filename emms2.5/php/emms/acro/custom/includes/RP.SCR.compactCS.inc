<?php

$data = WEBPAGE::$dbh->getAll(sprintf("select
    lmd.master_id,
    l.xp_cancel_date,
    l.xp_num_pmt,
    lt.payment_frequency,
    lcd.xp_pmt_date,
    lcd.loan_id,
    lcd.xp_pmt,
    lcd.cn_delay,
    lcd.cn_penalties,
    lm.program_id, 
    prg.program,
    lf.fund,
    lm.zone_id, z.short_name,
    concat(c.first,' ',c.last) cliente,
    c.code govID,
    s.name grupo,
    u.username,
    count(p.id)+1 cuota,
    rn.p_pmt_receipt_flag_a pmt_receipt_flag_a,
    rn.p_pmt_receipt_flag_b pmt_receipt_flag_b
from
    (
    tblLoansCurrentData lcd,
    tblLoansMasterDetails lmd,
    tblLoansMaster lm,
    tblLoans l,
    tblLoanTypes lt,
    tblClients c,
    tblPrograms prg,
    tblZones z,
    tblUsers u,
    view_loan_funds lf
    )
left join
    tblSocieties s on s.id = c.society_id
left join
    tblPayments p on p.loan_id = lcd.loan_id and p.special = 0
left join
    (select rnp.loan_id p_loan_id, count(rnr.flag_a) p_pmt_receipt_flag_a, count(rnr.flag_b) p_pmt_receipt_flag_b FROM tblPayments rnp, tblReceipts rnr, tblLinkReceiptsPayments rnlrp, tblLoansCurrentData rnlcd where rnlcd.loan_id = rnp.loan_id and rnlcd.xp_pmt_date <= date_add(now(), interval 7 day) and rnr.flag_a = '1' and rnr.flag_b = '1' and rnlrp.payment_id = rnp.id and rnlrp.receipt_id = rnr.id group by rnp.loan_id) rn on rn.p_loan_id = lcd.loan_id
where
        lcd.xp_pmt_date <= date_add(now(), interval 1 month)
    and lmd.loan_id = lcd.loan_id
    and lm.id = lmd.master_id
    and l.id = lcd.loan_id
    and lt.id = l.loan_type_id
    and c.id = l.client_id
    and lf.id = l.id
    and prg.id = lm.program_id
    and z.id = lm.zone_id
    and u.id = l.advisor_id
group by
    lcd.loan_id
order by
    lcd.loan_id"));

$head = array('id'=>'ID','client'=>'Cliente','num_pmt'=>'No cuota','xp_num_pmt'=>'Total de cuotas','payment_frequency'=>'Frec. Pagos','xp_pmt_date'=>'Fecha pago','xp_cancel_date'=>'Fecha vencimiento','xp_pmt'=>'Monto cuota','cn_penalties'=>'Punitorios','pmt_receipt_flag_a'=>WEBPAGE::$conf['app']['pmt_receipt_flag_a'] ? WEBPAGE::$conf['app']['pmt_receipt_flag_a'] : '-','pmt_receipt_flag_b'=>WEBPAGE::$conf['app']['pmt_receipt_flag_b'] ? WEBPAGE::$conf['app']['pmt_receipt_flag_b'] : '-','username'=>'Asesor','program'=>'Programa','fund'=>'Fondo','branch'=>'Sucursal','print'=>'Imprimir');

foreach($data as $key => $val) {
  $master[$val['master_id']]['id']                 = $val['master_id'];
  $master[$val['master_id']]['client']             = $counter[$val['master_id']] ? $val['grupo'] : $val['cliente'].' : '.$val['govID'];
//  $master[$val['master_id']]['client']           = $val['grupo'] ? $val['grupo'] : $val['cliente'];
  $master[$val['master_id']]['num_pmt']            = $val['cuota'];
  $master[$val['master_id']]['xp_num_pmt']         = $val['xp_num_pmt'];
  $master[$val['master_id']]['payment_frequency']  = $val['payment_frequency'];
  $master[$val['master_id']]['xp_pmt_date']        = $val['xp_pmt_date'];
  $master[$val['master_id']]['xp_cancel_date']     = $val['xp_cancel_date'];
  $master[$val['master_id']]['xp_pmt']            += $val['xp_pmt'];
  $master[$val['master_id']]['cn_penalties']      += $val['cn_penalties'];
  $master[$val['master_id']]['pmt_receipt_flag_a'] = WEBPAGE::$conf['app']['pmt_receipt_flag_a'] ? ( $val['pmt_receipt_flag_a'] ? $val['pmt_receipt_flag_a'] : 0 ) : '-';
  $master[$val['master_id']]['pmt_receipt_flag_b'] = WEBPAGE::$conf['app']['pmt_receipt_flag_b'] ? ( $val['pmt_receipt_flag_b'] ? $val['pmt_receipt_flag_b'] : 0 ) : '-';
  $master[$val['master_id']]['username']           = $val['username'];
  $master[$val['master_id']]['program']            = $val['program'];
  $master[$val['master_id']]['fund']               = $val['fund'];
  $master[$val['master_id']]['branch']             = $val['short_name'];

  $master[$val['master_id']]['print']             = sprintf('<a href="index.custom.php?scr_name=RP.SCR.receipt&advisor=%s&client=%s&xp_pmt_date=%s&num_pmt=%s&xp_num_pmt=%s&xp_pmt=%s">Imprimir</a>',$master[$val['master_id']]['username'],$master[$val['master_id']]['client'],$master[$val['master_id']]['xp_pmt_date'],$master[$val['master_id']]['num_pmt'],$master[$val['master_id']]['xp_num_pmt'],$master[$val['master_id']]['xp_pmt']);

  $counter[$val['master_id']]++;

  }


$tpl->setVariable('chart', count($master) ? WEBPAGE::printchart($master,$head) : WEBPAGE::$gt['noData']);

?>
