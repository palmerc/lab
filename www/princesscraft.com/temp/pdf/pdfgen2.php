<?php
require('fpdf.php');

class PDF extends FPDF
{
    //Page header
    //function Header($image,$address,$phone,$fax,$email,$webaddr)
    function Header()
    {
        //Logo
        $this->Image('pc_invoice_logo.png',10,8,0,0,'PNG','http://princesscraft.com');
        
        //Setup the address
        $this->SetFont('Arial','B',10);
        //Title
        $this->Cell(35);
        $this->Cell(35,11.5,'',0,1,'L');
        $this->Cell(35);
        $this->Cell(35,1.2,'Princess Craft Campers and Trailers',0,1,'L');
        $this->Ln();
        $this->SetFont('Arial','B',8);
        $this->Cell(35);
        $this->Cell(30,1.2,'102 N 1st St',0,1,'L');
        $this->Ln();
        $this->Cell(35);
        $this->Cell(30,1.2,'Pflugerville, TX 78660-2754',0,1,'L');
        $this->Ln();
        $this->Cell(35);
        $this->Cell(30,1.2,'Toll-Free: (800) 338-7123',0,1,'L');
        $this->Ln();
        $this->Cell(35);
        $this->Cell(30,1.2,'Phone: (512) 251-4536',0,1,'L');
        $this->Ln();
        $this->Cell(35);
        $this->Cell(30,1.2,'Fax: (512) 251-3134',0,1,'L');
        $this->Ln();
        $this->Cell(35);
        $this->Cell(9,1.2,'Email: ',0,0,'L');
        $this->Cell(0,1.2,'sales@princesscraft.com',0,0,'L',0,'mailto:sales@princesscraft.com');
        $this->Ln();
    }
    
    //Page footer
    function Footer()
    {
        //Position at 1.5 cm from bottom
        $this->SetY(-15);
        //Arial italic 8
        $this->SetFont('Arial','I',8);
        //Page number
        $this->Cell(0,10,'Page '.$this->PageNo().'/{nb}',0,0,'C');
    }
    
    function Code39($xpos, $ypos, $code, $baseline=0.5, $height=5){
    
        $wide = $baseline * 3;
        $narrow = $baseline;
        $gap = $narrow;
    
        $barChar['0'] = 'nnnwwnwnn';
        $barChar['1'] = 'wnnwnnnnw';
        $barChar['2'] = 'nnwwnnnnw';
        $barChar['3'] = 'wnwwnnnnn';
        $barChar['4'] = 'nnnwwnnnw';
        $barChar['5'] = 'wnnwwnnnn';
        $barChar['6'] = 'nnwwwnnnn';
        $barChar['7'] = 'nnnwnnwnw';
        $barChar['8'] = 'wnnwnnwnn';
        $barChar['9'] = 'nnwwnnwnn';
        $barChar['A'] = 'wnnnnwnnw';
        $barChar['B'] = 'nnwnnwnnw';
        $barChar['C'] = 'wnwnnwnnn';
        $barChar['D'] = 'nnnnwwnnw';
        $barChar['E'] = 'wnnnwwnnn';
        $barChar['F'] = 'nnwnwwnnn';
        $barChar['G'] = 'nnnnnwwnw';
        $barChar['H'] = 'wnnnnwwnn';
        $barChar['I'] = 'nnwnnwwnn';
        $barChar['J'] = 'nnnnwwwnn';
        $barChar['K'] = 'wnnnnnnww';
        $barChar['L'] = 'nnwnnnnww';
        $barChar['M'] = 'wnwnnnnwn';
        $barChar['N'] = 'nnnnwnnww';
        $barChar['O'] = 'wnnnwnnwn';
        $barChar['P'] = 'nnwnwnnwn';
        $barChar['Q'] = 'nnnnnnwww';
        $barChar['R'] = 'wnnnnnwwn';
        $barChar['S'] = 'nnwnnnwwn';
        $barChar['T'] = 'nnnnwnwwn';
        $barChar['U'] = 'wwnnnnnnw';
        $barChar['V'] = 'nwwnnnnnw';
        $barChar['W'] = 'wwwnnnnnn';
        $barChar['X'] = 'nwnnwnnnw';
        $barChar['Y'] = 'wwnnwnnnn';
        $barChar['Z'] = 'nwwnwnnnn';
        $barChar['-'] = 'nwnnnnwnw';
        $barChar['.'] = 'wwnnnnwnn';
        $barChar[' '] = 'nwwnnnwnn';
        $barChar['*'] = 'nwnnwnwnn';
        $barChar['$'] = 'nwnwnwnnn';
        $barChar['/'] = 'nwnwnnnwn';
        $barChar['+'] = 'nwnnnwnwn';
        $barChar['%'] = 'nnnwnwnwn';
    
        $this->SetFont('Arial','',10);
        $this->Text($xpos, $ypos + $height + 4, $code);
        $this->SetFillColor(0);
        $widths = array(
            'n' => $narrow,
            'w' => $wide,
        );
    
        $code = '*'.strtoupper($code).'*';
        foreach(chars($code) as $char) {
            $charComponents = $barChar[$char] or
                $this->Error('Invalid character in barcode: '.$char);
            
            foreach(chars($charComponents) as $i=>$c) {
                $lineWidth = $widths[$c] or
                    $this->Error('Unknown code '.$c);
                
                if($i % 2 == 0) {
                    $this->Rect($xpos, $ypos, $lineWidth, $height, 'F');
                }
                $xpos += $lineWidth;
            }
            $xpos += $gap;
        }
    } //End of Code39

    function Code128($xpos, $ypos, $code_type='b', $code, $baseline=0.5, $height=5){
        $narrow = $baseline;
        $medium = $baseline * 2;
        $wide = $baseline * 3;
        $ultra = $baseline * 4;
        
        $widths = array(
            '1' => $narrow,
            '2' => $medium,
            '3' => $wide,
            '4' => $ultra,
        );
    
        //Start and Stop codes
        $starta = '211412';
        $startb = '211214';
        $startc = '211232';
        $stop = '2331112';
    
        $code_types = array(
            'a' => $starta,
            'b' => $startb,
            'c' => $startc,
        );
    
        //Common Code A and B Characters
/*        $codeab_common_characters = array(
            ' ' => '212222',
            '!' => '222122',
            '"' => '222221',
            '#' => '121223',
            '$' => '121322',
            '%' => '131222',
            '&' => '122213',
            '\'' => '122312',
            '(' => '132212',
            ')' => '221213',
            '*' => '221312',
            '+' => '231212',
            ',' => '112232',
            '-' => '122132',
            '.' => '122231',
            '/' => '113222',
            '0' => '123122',
            '1' => '123221',
            '2' => '223211',
            '3' => '221132',
            '4' => '221231',
            '5' => '213212',
            '6' => '223112',
            '7' => '312131',
            '8' => '311222',
            '9' => '321122',
            ':' => '321221',
            ';' => '312212',
            '<' => '322112',
            '=' => '322211',
            '>' => '212123',
            '?' => '212321',
            '@' => '232121',
            'A' => '111323',
            'B' => '131123',
            'C' => '131321',
            'D' => '112313',
            'E' => '132113',
            'F' => '132311',
            'G' => '211313',
            'H' => '231113',
            'I' => '231311',
            'J' => '112133',
            'K' => '112331',
            'L' => '132131',
            'M' => '113123',
            'N' => '113321',
            'O' => '133121',
            'P' => '313121',
            'Q' => '211331',
            'R' => '231131',
            'S' => '213113',
            'T' => '213311',
            'U' => '213131',
            'V' => '311123',
            'W' => '311321',
            'X' => '331121',
            'Y' => '312113',
            'Z' => '312311',
            '[' => '332111',
            '\\' => '314111',
            ']' => '221411',
            '^' => '431111',
            '_' => '111224',
            '`' => '111422',
        );*/
        
        //Code B Characters
        $codeb_characters = array(
            ' ' => '212222',
            '!' => '222122',
            '"' => '222221',
            '#' => '121223',
            '$' => '121322',
            '%' => '131222',
            '&' => '122213',
            '\'' => '122312',
            '(' => '132212',
            ')' => '221213',
            '*' => '221312',
            '+' => '231212',
            ',' => '112232',
            '-' => '122132',
            '.' => '122231',
            '/' => '113222',
            '0' => '123122',
            '1' => '123221',
            '2' => '223211',
            '3' => '221132',
            '4' => '221231',
            '5' => '213212',
            '6' => '223112',
            '7' => '312131',
            '8' => '311222',
            '9' => '321122',
            ':' => '321221',
            ';' => '312212',
            '<' => '322112',
            '=' => '322211',
            '>' => '212123',
            '?' => '212321',
            '@' => '232121',
            'A' => '111323',
            'B' => '131123',
            'C' => '131321',
            'D' => '112313',
            'E' => '132113',
            'F' => '132311',
            'G' => '211313',
            'H' => '231113',
            'I' => '231311',
            'J' => '112133',
            'K' => '112331',
            'L' => '132131',
            'M' => '113123',
            'N' => '113321',
            'O' => '133121',
            'P' => '313121',
            'Q' => '211331',
            'R' => '231131',
            'S' => '213113',
            'T' => '213311',
            'U' => '213131',
            'V' => '311123',
            'W' => '311321',
            'X' => '331121',
            'Y' => '312113',
            'Z' => '312311',
            '[' => '332111',
            '\\' => '314111',
            ']' => '221411',
            '^' => '431111',
            '_' => '111224',
            '`' => '111422',
            '`' => '111422',
            'a' => '121124',
            'b' => '121421',
            'c' => '141122',
            'd' => '141221',
            'e' => '112214',
            'f' => '112412',
            'g' => '122114',
            'h' => '122411',
            'i' => '142112',
            'j' => '142211',
            'k' => '241211',
            'l' => '221114',
            'm' => '413111',
            'n' => '241112',
            'o' => '134111',
            'p' => '111242',
            'q' => '121142',
            'r' => '121241',
            's' => '114212',
            't' => '124112',
            'u' => '124211',
            'v' => '411212',
            'w' => '421112',
            'x' => '421211',
            'y' => '212141',
            'z' => '214121',
            '{' => '412121',
            '|' => '111143',
            '}' => '111341',
            '~' => '131141',
        );
        
        //Code C Characters
        $codec_characters = array(
            '01' => '222122', 
            '02' => '222221', 
            '03' => '121223', 
            '04' => '121322', 
            '05' => '131222', 
            '06' => '122213', 
            '07' => '122312', 
            '08' => '132212', 
            '09' => '221213',
            '10' => '221312',
            '11' => '231212',
            '12' => '112232',
            '13' => '122132',
            '14' => '122231',
            '15' => '113222',
            '16' => '123122',
            '17' => '123221',
            '18' => '223211',
            '19' => '221132',
            '20' => '221231',
            '21' => '213212',
            '22' => '223112',
            '23' => '312131',
            '24' => '311222',
            '25' => '321122',
            '26' => '321221',
            '27' => '312212',
            '28' => '322112',
            '29' => '322211',
            '30' => '212123', 
            '31' => '212321', 
            '32' => '232121', 
            '33' => '111323', 
            '34' => '131123',
            '35' => '131321',
            '36' => '112313',
            '37' => '132113',
            '38' => '132311',
            '39' => '211313',
            '40' => '231113',
            '41' => '231311', 
            '42' => '112133', 
            '43' => '112331', 
            '44' => '132131', 
            '45' => '113123', 
            '46' => '113321', 
            '47' => '133121', 
            '48' => '313121', 
            '49' => '211331',
            '50' => '231131', 
            '51' => '213113', 
            '52' => '213311', 
            '53' => '213131', 
            '54' => '311123', 
            '55' => '311321', 
            '56' => '331121', 
            '57' => '312113', 
            '58' => '312311', 
            '59' => '332111',
            '60' => '314111', 
            '61' => '221411',
            '62' => '431111',
            '63' => '111224',
            '64' => '111422',
            '65' => '121124',
            '66' => '121421',
            '67' => '141122', 
            '68' => '141221', 
            '69' => '112214',
            '70' => '112412', 
            '61' => '122114', 
            '72' => '122411', 
            '73' => '142112', 
            '74' => '142211',
            '75' => '241211', 
            '76' => '221114', 
            '77' => '413111', 
            '78' => '241112', 
            '79' => '134111',
            '80' => '111242', 
            '81' => '121142',
            '82' => '121241', 
            '83' => '114212',
            '84' => '124112', 
            '85' => '124211', 
            '86' => '411212', 
            '87' => '421112', 
            '88' => '421211', 
            '89' => '212141',
            '90' => '214121',
            '91' => '412121',
            '92' => '111143',
            '93' => '111341',
            '94' => '131141',
            '95' => '114113',
            '96' => '114311',	
            '97' => '411113',	
            '98' => '411311',	
            '99' => '113141',	
        );
        
        $master_codes = array(
            'ab' => $codeab_common_characters,
            //'a' => $codea_characters,
            'b' => $codeb_characters,
            'c' => $codec_characters,
        );
        
        if($code_type == "c") $master_codes['ab'] = array();
    
        /*
        100(Hex84)	CODEB	FNC4	CODEB	'114131'
        101(Hex85)	FNC4	CODEA	CODEA	'311141'
        102(Hex86)	FNC1	FNC1	FNC1	'411131'
        */
        
        $this->SetFont('Arial','',10);
        $this->Text($xpos, $ypos + $height + 4, $code);
        $this->SetFillColor(0);
    
        $start = $code_types[$code_type] or 
            $this->Error('Invalid start code type '.$code_type);
        
        if($code_type == "c") {
            $chars_func = "codec_chars";
        } else {
            $chars_func = "chars";
        }
        
        //$code = $start.$code.$stop;
        foreach(chars($code) as $char) {
            $charComponents = $master_codes[$code_type][$char] or
                $master_codes['ab'][$char] or
                $this->Error('Invalid character in barcode: '.$char);
            //errory($char);
    //        $this->Error('Char: '.$char);
            foreach(chars($charComponents) as $i=>$c) {
                $lineWidth = $widths[$c] or
                    $this->Error('Unknown code '.$c);
                //$this->Error('lineWidth: '.$lineWidth);
              //  errory($i.$c);
                if($i % 2 == 0) {
                    $this->Rect($xpos, $ypos, $lineWidth, $height, 'F');
                }
                $xpos += $lineWidth;
            }
        }
    } //End of Code128
} //End of Class

//Instanciation of inherited class - Calls Header and Footer Function
$pdf=new PDF('P','mm','Letter');
$pdf->AliasNbPages();
$pdf->AddPage();
// Sets the font as 12 point Arial
$pdf->SetFont('Arial','',12);
$pdf->Code39(80,40,'Cameron Palmer',0.25,10);
$pdf->Code128(80,80,'b','Cameron Palmer',0.25,10);
//$pdf->Code39(80,40,'Cameron Palmer',1,10);
for($i=1;$i<=40;$i++)
    $pdf->Cell(0,10,"Printing line number ".$i,0,1);
$pdf->Output();

function errory($things){
    print_r("$things\n");
}

/* Equivalent to Perl's split('',$str) function */
function chars($str) {
    $ret = array();
    for($i=0;$i<strlen($str);$i++) {
        $ret[] = $str{$i};
    }
    return $ret;
}

/* Counts off pairs of characters rather than singles */
function codec_chars($str) {
    $ret = array();
    for($i=0;$i<strlen($str);$i+=2) {
        $ret[] = substr($str, $i, 2);
    }
    return $ret;
}
?>
