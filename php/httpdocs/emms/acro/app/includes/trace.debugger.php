<?php
/********************************
Trace Debugger Library ver. 1.5
********************************/


// Activating trace debugging true/false

define('TRACE_DEBUGGING',true);

/* Documentation: 

Defining path for debug log file - 

Create a writeable folder 'debug' under 'promerica', add a proper .htaccess file to avoid web reading. 

Example:
for ~/public_html/promerica/debug/

should be:
define(LOCAL_PATH,'/promerica/debug/');

For e-MMS 2.0 developing i place trace.debugger.php in eird/includes and add the following line as the top most in index.php , index.popup.php or other namespace entry point.

include_once 'includes/trace.debugger.php'; 

Notice i'm using 'include_once' instead of 'require_once' to avoid breaking the script in case there's problem with the trace.debugger.php loading. (critical on production servers).

Standard Usage:

trace($a);

dumps $a contents (print_r formatted if array, object variable) to ['document_root']/promerica/debug/default.log2.txt file overriding previous content..

trace($a,0);

dumps $a contents (print_r formatted if array, object variable) to ['document_root']/promerica/debug/default.log2.txt file appending to existing content..

trace($a,0,'monto_pendiente');

dumps $a contents (print_r formatted if array, object variable) to ['document_root']/promerica/debug/default.log2.txt file appending to existing content, and labeling entry as 'monto_pendiente'.

trace($a,0,'monto_pendiente','variables_financieras');

dumps $a contents (print_r formatted if array, object variable) to ['document_root']/promerica/debug/variables_financieras.log2.txt file appending to existing content, and labeling entry as 'monto_pendiente'. Several namespaces could be monitored in differents log/trace files this way. 


Safe usage to avoid breaking the PHP script if trace() function is not in namespace. (useful in production servers).
if (function_exists('trace')) {
   trace($a);
}

For local developement i recommend http://follow.sourceforge.net/ to monitor several log files.

*/
define('LOCAL_PATH','/app/debug/');
// define(DEBUG_LOGFILE_PATH,$_SERVER['DOCUMENT_ROOT'] . LOCAL_PATH);

function trace($variable_to_trace,$eraseFlag=1,$label='default',$namespace='default') {
if(TRACE_DEBUGGING) {

// Defining log filename

$debug_logfile_path = $_SERVER['DOCUMENT_ROOT'].LOCAL_PATH.$namespace.'.'.'log2.txt';

// Getting data from the variable being debugged

if(is_array($variable_to_trace)) {

$data=print_r($variable_to_trace,true);}

elseif (is_object($variable_to_trace)) {$data=print_r(get_object_vars($variable_to_trace),true);}

elseif (is_bool($variable_to_trace)) {

    $data=var_export($variable_to_trace,true);
} 


elseif (is_resource($variable_to_trace)) {

    $data=var_export(get_resource_type($variable_to_trace),true);
}

else {
 
 $data=var_export($variable_to_trace,true);
 
}

//delete existing log file to avoid browsing older logs
if ($eraseFlag) {
if(file_exists($debug_logfile_path)) {
unlink($debug_logfile_path);
}
}

// find caller data (file,line)

$location = debug_backtrace();

		while($d = array_pop($location)) { $count++;
			if ((strToLower($d['function']) == 'trace') || (strToLower(@$d['class']) == 'trace')) {
				break;
				}
}


// Formatting log output

if (is_scalar($variable_to_trace)) {


if (isset($d['file'])) {
$output.="file:".$d['file']." >";
}

if (isset($d['line'])) {
$output.="line:".$d['line']." >";
}


if (isset($d['function'])) {
$output.="function:".$d['function']." >";
}


if (isset($d['class'])) {
$output.="class:".$d['class']." >";
}

if (isset($d['method'])) {
$output.="method:".$d['method']." >";
}

$output.="$label:$data"."\n";

} else {
$output.= "=============***================\n";
$output.="levels:".$count."\n";
if (isset($d['file'])) {
$output.="file:".$d['file']."\n";
}

if (isset($d['line'])) {
$output.="line:".$d['line']."\n";
}


if (isset($d['function'])) {
$output.="function:".$d['function']."\n";
}


if (isset($d['class'])) {
$output.="class:".$d['class']."\n";
}

if (isset($d['method'])) {
$output.="method:".$d['method']."\n";
}


// if (isset($location['label'])) {
// 
// $label = $location['label'];} else {
// 
// $label = $namespace;
// 
// }




$output.= "$label:$data\n";
$output.= date("Y-m-d H:i:s")."\n";
$output.= "======********************======\n";
}


error_log($output,3,$debug_logfile_path);
}
}

?>