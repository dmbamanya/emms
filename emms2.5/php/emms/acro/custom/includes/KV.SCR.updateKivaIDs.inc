<?php
$dfile = 'kivaIDs.csv';
	if (file_exists($dfile)) {
		$fp = fopen ($dfile, "r");
		while ( $cdata = fgetcsv($fp, 1000, ",") ) {
			$data[$cdata[0]] = $cdata[1];
			}
		fclose($fp);
		} else {
		$data = array();
		}
$c = 0;
foreach($data as $master_id => $kiva_id) {
   WEBPAGE::$dbh->query(sprintf('update tblLoansMaster set kiva_id = "%s" where id = "%s"',$kiva_id,$master_id));
   $c++;
// printf('update tblLoansMaster set kiva_id = "%s" where id = "%s"',$kiva_id,$master_id); echo '<br />';
    }
echo 'Master ID records inserted: '. $c;
?>