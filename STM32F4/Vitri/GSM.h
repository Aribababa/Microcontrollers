
/* Comandos para el módulo */
#define Check				"AT\r"
#define Quotation_Marks		"\""
#define Call				"ATD"
#define Answer_Call			"ATA\r"
#define HangUp_Call			"ATH\r"
#define Quality_Signal		"AT+CSQ\r"
#define Modem_Information	"ATI\r"
#define SendSMS_Mode		"AT+CMGF=1\r"
#define SetSMS				"AT+CMGS="
#define Terminator			"\r"
#define SMS_NewIdicator		"AT+CNMI=2,2,0,0,0\r"
#define Read_SMS(X)			"AT+CMGR=3"+(X+0x30)+"\r"
#define FinishSMS			0x1A

/* Estados para el envio de mensaje del módulo  */
#define SMS_IDLE_STATE    	0
#define SMS_MODE_STATE    	1
#define SMS_NUMBER_STATE   	2
#define SMS_CONTENT_STATE 	3

/* Aqui se guarda los datos que se reciben */
extern unsigned char RxBuffer[128];
extern unsigned char Incoming_call_flag;
extern unsigned char GSM_OK_Flag;
/* Inicia el módulo GSM */
void GSM_init(void);

/* Envia un dato por el puerto serial hacia el GSM */
void GSM_Send(volatile char *s);

/* Activa la máquina de estados que controla el envio de mensajes */
void GSM_SendSMS(volatile char *PhoneNumber, volatile char *Content);

/* Hace una llamada al numero que se pasa como parámetro */
void GSM_Call(volatile char *PhoneNumber);

/* Contiene los estados de la máquina de estados del GSM */
void GSM_SMS_StateMachine(void);

