package org.myrobotlab.service;

import java.io.IOException;

import org.myrobotlab.framework.Service;
import org.myrobotlab.framework.ServiceType;
import org.myrobotlab.logging.Level;
import org.myrobotlab.logging.LoggerFactory;
import org.myrobotlab.logging.Logging;
import org.myrobotlab.logging.LoggingFactory;
import org.myrobotlab.openni.OpenNiData;
import org.myrobotlab.openni.Skeleton;
import org.myrobotlab.service.interfaces.SpeechSynthesis;
import org.slf4j.Logger;

/**
 * 
 * Sweety - The sweety robot service. Maintained by \@beetlejuice
 *
 */
public class Sweety extends Service {

  private static final long serialVersionUID = 1L;

  public final static Logger log = LoggerFactory.getLogger(Sweety.class);

  transient public Arduino arduino;
  transient public WebkitSpeechRecognition ear;
  transient public WebGui webGui;
  transient public SpeechSynthesis mouth;
  transient public Tracking leftTracker;
  transient public Tracking rightTracker;
  transient public ProgramAB chatBot;
  transient public OpenNi openni;
  transient public Pid pid;
  transient public HtmlFilter htmlFilter;

  transient Servo leftForearm;
  transient Servo rightForearm;
  transient Servo rightShoulder;
  transient Servo leftShoulder;
  transient Servo rightArm;
  transient Servo neck;
  transient Servo leftEye;
  transient Servo leftArm;
  transient Servo rightEye;
  transient Servo rightHand;
  transient Servo rightWrist;
  transient Servo leftHand;
  transient Servo leftWrist;

  transient public UltrasonicSensor USfront;
  transient public UltrasonicSensor USfrontRight;
  transient public UltrasonicSensor USfrontLeft;
  transient public UltrasonicSensor USback;
  transient public UltrasonicSensor USbackRight;
  transient public UltrasonicSensor USbackLeft;

  boolean copyGesture = false;
  boolean firstSkeleton = true;
  boolean saveSkeletonFrame = false;

  // arduino pins variables
  int rightMotorDirPin = 2;
  int rightMotorPwmPin = 3;
  int leftMotorDirPin = 4;
  int leftMotorPwmPin = 5;

  int backUltrasonicTrig = 22;
  int backUltrasonicEcho = 23;
  int back_leftUltrasonicTrig = 24;
  int back_leftUltrasonicEcho = 25;
  int back_rightUltrasonicTrig = 26;
  int back_rightUltrasonicEcho = 27;
  int front_leftUltrasonicTrig = 28;
  int front_leftUltrasonicEcho = 29;
  int frontUltrasonicTrig = 30;
  int frontUltrasonicEcho = 31;
  int front_rightUltrasonicTrig = 32;
  int front_rightUltrasonicEcho = 33;

  int SHIFT = 47;
  int LATCH = 48;
  int DATA = 49;

  // variable for servomotors infos ( min, max, neutral )
  int leftForearmMin = 70;
  int rightForearmMin = 0;
  int rightShoulderMin = 2;
  int leftShoulderMin = 5;
  int rightArmMin = 0;
  int neckMin = 55;
  int leftEyeMin = 25;
  int leftArmMin = 70;
  int rightEyeMin = 80;
  int rightHandMin = 10;
  int rightWristMin = 0;
  int leftHandMin = 80;
  int leftWristMin = 0;

  int leftForearmMax = 155;
  int rightForearmMax = 80;
  int rightShoulderMax = 155;
  int leftShoulderMax = 160;
  int rightArmMax = 80;
  int neckMax = 105;
  int leftEyeMax = 125;
  int leftArmMax = 160;
  int rightEyeMax = 180;
  int rightHandMax = 75;
  int rightWristMax = 180;
  int leftHandMax = 150;
  int leftWristMax = 180;

  int leftForearmNeutral = 150;
  int rightForearmNeutral = 5;
  int rightShoulderNeutral = 2;
  int leftShoulderNeutral = 160;
  int rightArmNeutral = 2;
  int neckNeutral = 75;
  int leftEyeNeutral = 75;
  int leftArmNeutral = 155;
  int rightEyeNeutral = 127;
  int rightHandNeutral = 10;
  int rightWristNeutral = 116;
  int leftHandNeutral = 150;
  int leftWristNeutral = 85;

  // variables for speak / mouth sync
  public int delaytime = 3;
  public int delaytimestop = 5;
  public int delaytimeletter = 1;

  public static void main(String[] args) {
    LoggingFactory.getInstance().configure();
    LoggingFactory.getInstance().setLevel(Level.INFO);

    try {

      Runtime.start("sweety", "Sweety");

    } catch (Exception e) {
      Logging.logError(e);
    }
  }

  public Sweety(String n) {
    super(n);
  }

  /**
   * Attach the servos to arduino pins
 * @throws Exception 
   */
  public void attach() throws Exception {
	  /* OLD  WAY
    rightForearm.attach(arduino.getName(), 34);
    leftForearm.attach(arduino.getName(), 35);
    rightShoulder.attach(arduino.getName(), 36);
    leftShoulder.attach(arduino.getName(), 37);
    rightArm.attach(arduino.getName(), 38);
    neck.attach(arduino.getName(), 39);
    leftEye.attach(arduino.getName(), 40);
    leftArm.attach(arduino.getName(), 41);
    rightEye.attach(arduino.getName(), 42);
    leftHand.attach(arduino.getName(), 43);
    rightWrist.attach(arduino.getName(), 44);
    leftWrist.attach(arduino.getName(), 45);
    rightHand.attach(arduino.getName(), 46);
    */
    
	  // NEW WAY
    arduino.servoAttach(rightForearm, 34);
    arduino.servoAttach(leftForearm, 35);
    arduino.servoAttach(rightShoulder, 36);
    arduino.servoAttach(leftShoulder, 37);
    arduino.servoAttach(rightArm, 38);
    arduino.servoAttach(neck, 39);
    arduino.servoAttach(leftEye, 40);
    arduino.servoAttach(leftArm, 41);
    arduino.servoAttach(rightEye, 42);
    arduino.servoAttach(leftHand, 43);
    arduino.servoAttach(rightWrist, 44);
    arduino.servoAttach(leftWrist, 45);
    arduino.servoAttach(rightHand, 46);
  }

  /**
   * Connect the arduino to a COM port . Exemple : connect("COM8")
 * @throws IOException 
   */
  public void connect(String port) throws IOException {
    arduino.connect(port);
    sleep(2000);
    arduino.pinMode(SHIFT, Arduino.OUTPUT);
    arduino.pinMode(LATCH, Arduino.OUTPUT);
    arduino.pinMode(DATA, Arduino.OUTPUT);
  }

  /**
   * detach the servos from arduino pins
   */
  public void detach() {
    rightForearm.detach();
    leftForearm.detach();
    rightShoulder.detach();
    leftShoulder.detach();
    rightArm.detach();
    leftArm.detach();
    neck.detach();
    leftEye.detach();
    rightEye.detach();
    rightHand.detach();
    rightWrist.detach();
    leftHand.detach();
    leftWrist.detach();
  }

  // TODO Correct the head function for new head
  /**
   * Move the head . Use : head(neckAngle, rightEyeAngle, leftEyeAngle -1 mean
   * "no change"
   */
  public void setHeadPosition(Integer neckAngle, Integer rightEyeAngle, Integer leftEyeAngle) {

    if (neckAngle == -1) {
      neckAngle = neck.getPos();
    }
    if (rightEyeAngle == -1) {
      rightEyeAngle = rightEye.getPos();
    }
    if (leftEyeAngle == -1) {
      leftEyeAngle = leftEye.getPos();
    }

    neck.moveTo(neckAngle);
    rightEye.moveTo(rightEyeAngle);
    leftEye.moveTo(leftEyeAngle);
  }

  /**
   * Move the right arm . Use : leftArm(shoulder angle, arm angle, forearm
   * angle, wrist angle, hand angle) -1 mean "no change"
   */
  public void setRightArmPosition(Integer shoulderAngle, Integer armAngle, Integer forearmAngle, Integer wristAngle, Integer handAngle) {

    // TODO protect against self collision
    if (shoulderAngle == -1) {
      shoulderAngle = rightShoulder.getPos();
    }
    if (armAngle == -1) {
      armAngle = rightArm.getPos();
    }
    if (forearmAngle == -1) {
      forearmAngle = rightForearm.getPos();
    }
    if (wristAngle == -1) {
      wristAngle = rightWrist.getPos();
    }
    if (handAngle == -1) {
      handAngle = rightHand.getPos();
    }

    rightShoulder.moveTo(shoulderAngle);
    rightArm.moveTo(armAngle);
    rightForearm.moveTo(forearmAngle);
    rightWrist.moveTo(wristAngle);
    rightHand.moveTo(handAngle);
  }

  /**
   * Move the left arm . Use : leftArm(shoulder angle, arm angle, forearm angle,
   * wrist angle, hand angle) -1 mean "no change"
   */
  public void setLeftArmPosition(Integer shoulderAngle, Integer armAngle, Integer forearmAngle, Integer wristAngle, Integer handAngle) {
    // TODO protect against self collision with -> servoName.getPos()
    if (shoulderAngle == -1) {
      shoulderAngle = leftShoulder.getPos();
    }
    if (armAngle == -1) {
      armAngle = leftArm.getPos();
    }
    if (forearmAngle == -1) {
      forearmAngle = leftForearm.getPos();
    }
    if (wristAngle == -1) {
      wristAngle = leftWrist.getPos();
    }
    if (handAngle == -1) {
      handAngle = leftHand.getPos();
    }

    leftShoulder.moveTo(shoulderAngle);
    leftArm.moveTo(armAngle);
    leftForearm.moveTo(forearmAngle);
    leftWrist.moveTo(wristAngle);
    leftHand.moveTo(handAngle);
  }

  /**
   * Set the mouth attitude . choose : smile, notHappy, speechLess, empty.
   */
  public void mouthState(String value) {
    if (value == "smile") {
      myShiftOut("11011100");
    } else if (value == "notHappy") {
      myShiftOut("00111110");
    } else if (value == "speechLess") {
      myShiftOut("10111100");
    } else if (value == "empty") {
      myShiftOut("00000000");
    }

  }

  /**
   * drive the motors . Speed > 0 go forward . Speed < 0 go backward . Direction
   * > 0 go right . Direction < 0 go left
   */
  public void moveMotors(int speed, int direction) {
    int speedMin = 50; // min PWM needed for the motors
    boolean isMoving = false;
    int rightCurrentSpeed = 0;
    int leftCurrentSpeed = 0;

    if (speed < 0) { // Go backward
      arduino.analogWrite(rightMotorDirPin, 0);
      arduino.analogWrite(leftMotorDirPin, 0);
      speed = speed * -1;
    } else {// Go forward
      arduino.analogWrite(rightMotorDirPin, 255);
      arduino.analogWrite(leftMotorDirPin, 255);
    }

    if (direction > speedMin && speed > speedMin) {// move and turn to the
      // right
      if (isMoving) {
        arduino.analogWrite(rightMotorPwmPin, direction);
        arduino.analogWrite(leftMotorPwmPin, speed);
      } else {
        rightCurrentSpeed = speedMin;
        leftCurrentSpeed = speedMin;
        while (rightCurrentSpeed < speed && leftCurrentSpeed < direction) {
          if (rightCurrentSpeed < direction) {
            rightCurrentSpeed++;
          }
          if (leftCurrentSpeed < speed) {
            leftCurrentSpeed++;
          }
          arduino.analogWrite(rightMotorPwmPin, rightCurrentSpeed);
          arduino.analogWrite(leftMotorPwmPin, leftCurrentSpeed);
          sleep(20);
        }
        isMoving = true;
      }
    } else if (direction < (speedMin * -1) && speed > speedMin) {// move and
      // turn
      // to
      // the
      // left
      direction *= -1;
      if (isMoving) {
        arduino.analogWrite(leftMotorPwmPin, direction);
        arduino.analogWrite(rightMotorPwmPin, speed);
      } else {
        rightCurrentSpeed = speedMin;
        leftCurrentSpeed = speedMin;
        while (rightCurrentSpeed < speed && leftCurrentSpeed < direction) {
          if (rightCurrentSpeed < speed) {
            rightCurrentSpeed++;
          }
          if (leftCurrentSpeed < direction) {
            leftCurrentSpeed++;
          }
          arduino.analogWrite(rightMotorPwmPin, rightCurrentSpeed);
          arduino.analogWrite(leftMotorPwmPin, leftCurrentSpeed);
          sleep(20);
        }
        isMoving = true;
      }
    } else if (speed > speedMin) { // Go strait
      if (isMoving) {
        arduino.analogWrite(leftMotorPwmPin, speed);
        arduino.analogWrite(rightMotorPwmPin, speed);
      } else {
        int CurrentSpeed = speedMin;
        while (CurrentSpeed < speed) {
          CurrentSpeed++;
          arduino.analogWrite(rightMotorPwmPin, CurrentSpeed);
          arduino.analogWrite(leftMotorPwmPin, CurrentSpeed);
          sleep(20);
        }
        isMoving = true;
      }
    } else if (speed < speedMin && direction < speedMin * -1) {// turn left
      arduino.analogWrite(rightMotorDirPin, 255);
      arduino.analogWrite(leftMotorDirPin, 0);
      arduino.analogWrite(leftMotorPwmPin, speedMin);
      arduino.analogWrite(rightMotorPwmPin, speedMin);

    }

    else if (speed < speedMin && direction > speedMin) {// turn right
      arduino.analogWrite(rightMotorDirPin, 0);
      arduino.analogWrite(leftMotorDirPin, 255);
      arduino.analogWrite(leftMotorPwmPin, speedMin);
      arduino.analogWrite(rightMotorPwmPin, speedMin);
    } else {// stop
      arduino.analogWrite(leftMotorPwmPin, 0);
      arduino.analogWrite(rightMotorPwmPin, 0);
      isMoving = false;
    }

  }

  /**
   * Used to manage a shift register
   */
  private void myShiftOut(String value) {
    arduino.digitalWrite(LATCH, 0); // Stop the copy
    for (int i = 0; i < 8; i++) { // Store the data
      if (value.charAt(i) == '1') {
        arduino.digitalWrite(DATA, 1);
      } else {
        arduino.digitalWrite(DATA, 0);
      }
      arduino.digitalWrite(SHIFT, 1);
      arduino.digitalWrite(SHIFT, 0);
    }
    arduino.digitalWrite(LATCH, 1); // copy

  }

  /**
   * Move the servos to show asked posture
   */
  public void posture(String pos) {
    if (pos == "neutral") {
      setLeftArmPosition(leftShoulderNeutral, leftArmNeutral, leftForearmNeutral, leftWristNeutral, leftHandNeutral);
      setRightArmPosition(rightShoulderNeutral, rightArmNeutral, rightForearmNeutral, rightWristNeutral, rightHandNeutral);
      setHeadPosition(neckNeutral, rightEyeNeutral, leftEyeNeutral);
    }
    /*
     * Template else if (pos == ""){ setLeftArmPosition(, , , 85, 150);
     * setRightArmPosition(, , , 116, 10); setHeadPosition(75, 127, 75); }
     */
    // TODO correct angles for posture
    else if (pos == "yes") {
      setLeftArmPosition(0, 95, 136, 85, 150);
      setRightArmPosition(155, 55, 5, 116, 10);
      setHeadPosition(75, 127, 75);
    } else if (pos == "concenter") {
      setLeftArmPosition(37, 116, 85, 85, 150);
      setRightArmPosition(109, 43, 54, 116, 10);
      setHeadPosition(97, 127, 75);
    } else if (pos == "showLeft") {
      setLeftArmPosition(68, 63, 160, 85, 150);
      setRightArmPosition(2, 76, 40, 116, 10);
      setHeadPosition(85, 127, 75);
    } else if (pos == "showRight") {
      setLeftArmPosition(145, 79, 93, 85, 150);
      setRightArmPosition(80, 110, 5, 116, 10);
      setHeadPosition(75, 127, 75);
    } else if (pos == "handsUp") {
      setLeftArmPosition(0, 79, 93, 85, 150);
      setRightArmPosition(155, 76, 40, 116, 10);
      setHeadPosition(75, 127, 75);
    } else if (pos == "carryBags") {
      setLeftArmPosition(145, 79, 93, 85, 150);
      setRightArmPosition(2, 76, 40, 116, 10);
      setHeadPosition(75, 127, 75);
    }

  }

  @Override
  public Sweety publishState() {
    super.publishState();
    arduino.publishState();
    leftForearm.publishState();
    rightForearm.publishState();
    rightShoulder.publishState();
    leftShoulder.publishState();
    rightArm.publishState();
    neck.publishState();
    leftEye.publishState();
    leftArm.publishState();
    rightEye.publishState();
    rightHand.publishState();
    rightWrist.publishState();
    leftHand.publishState();
    leftWrist.publishState();
    return this;
  }

  /**
   * Say text and move mouth leds
   */
  public synchronized void saying(String text) { // Adapt mouth leds to words
    log.info("Saying :" + text);
    try {
      mouth.speak(text);
    } catch (Exception e) {
      Logging.logError(e);
    }
  }

  public synchronized void onStartSpeaking(String text) {
    sleep(15);
    boolean ison = false;
    String testword;
    String[] a = text.split(" ");
    for (int w = 0; w < a.length; w++) {
      testword = a[w];
      char[] c = testword.toCharArray();

      for (int x = 0; x < c.length; x++) {
        char s = c[x];

        if ((s == 'a' || s == 'e' || s == 'i' || s == 'o' || s == 'u' || s == 'y') && !ison) {

          myShiftOut("00011100");
          ison = true;
          sleep(delaytime);
          myShiftOut("00000100");
        } else if (s == '.') {
          ison = false;
          myShiftOut("00000000");
          sleep(delaytimestop);
        } else {
          ison = false;
          sleep(delaytimeletter); //
        }

      }

    }
  }

  public synchronized void onEndSpeaking(String utterance) {
    myShiftOut("00000000");
  }

  public void setdelays(Integer d1, Integer d2, Integer d3) {

    delaytime = d1;
    delaytimestop = d2;
    delaytimeletter = d3;
  }

  @Override
  public void startService() {
    super.startService();

    arduino = (Arduino) startPeer("arduino");
    chatBot = (ProgramAB) startPeer("chatBot");
    htmlFilter = (HtmlFilter) startPeer("htmlFilter");
    mouth = (SpeechSynthesis) startPeer("mouth");
    mouth.setLanguage("FR");
    mouth.setVoice("Antoine");
    ear = (WebkitSpeechRecognition) startPeer("ear");
    webGui = (WebGui) startPeer("webGui");
    subscribe(mouth.getName(), "publishStartSpeaking");
    subscribe(mouth.getName(), "publishEndSpeaking");

  }

  public void startServos() {
    leftForearm = (Servo) startPeer("leftForearm");
    rightForearm = (Servo) startPeer("rightForearm");
    rightShoulder = (Servo) startPeer("rightShoulder");
    leftShoulder = (Servo) startPeer("leftShoulder");
    rightArm = (Servo) startPeer("rightArm");
    neck = (Servo) startPeer("neck");
    leftEye = (Servo) startPeer("leftEye");
    leftArm = (Servo) startPeer("leftArm");
    rightEye = (Servo) startPeer("rightEye");
    rightHand = (Servo) startPeer("rightHand");
    rightWrist = (Servo) startPeer("rightWrist");
    leftHand = (Servo) startPeer("leftHand");
    leftWrist = (Servo) startPeer("leftWrist");

    leftForearm.setMinMax(leftForearmMin, leftForearmMax);
    rightForearm.setMinMax(rightForearmMin, rightForearmMax);
    rightShoulder.setMinMax(rightShoulderMin, rightShoulderMax);
    leftShoulder.setMinMax(leftShoulderMin, leftShoulderMax);
    rightArm.setMinMax(rightArmMin, rightArmMax);
    neck.setMinMax(neckMin, neckMax);
    leftEye.setMinMax(leftEyeMin, leftEyeMax);
    leftArm.setMinMax(leftArmMin, leftArmMax);
    rightEye.setMinMax(rightEyeMin, rightEyeMax);
    rightHand.setMinMax(rightHandMin, rightHandMax);
    rightWrist.setMinMax(rightWristMin, rightWristMax);
    leftHand.setMinMax(leftHandMin, leftHandMax);
    leftWrist.setMinMax(leftWristMin, leftWristMax);

  }

  /**
   * Start the tracking services
   */
  public void startTrack(String port, int leftCameraIndex, int rightCameraIndex) throws Exception {
    neck.detach();
    rightEye.detach();
    leftEye.detach();

    leftTracker = (Tracking) startPeer("leftTracker");
    /* OLD WAY
    leftTracker.y.setPin(39); // neck
    leftTracker.x.setPin(40); // right eye
    leftTracker.connect(port);
    */
    leftTracker.connect(port, 40, 39);

    leftTracker.pid.invert("y");
    leftTracker.opencv.setCameraIndex(leftCameraIndex);
    leftTracker.opencv.capture();

    rightTracker = (Tracking) startPeer("rightTracker");
    /* OLD WAY
    rightTracker.x.setPin(42); // right eye
    rightTracker.connect(port);
    rightTracker.y.setPin(50); // nothing
    */
    
    rightTracker.connect(port, 42, 50);
    
    rightTracker.pid.invert("y");
    rightTracker.opencv.setCameraIndex(rightCameraIndex);
    rightTracker.opencv.capture();
    saying("tracking activated.");
  }

  /**
   * Start the ultrasonic sensors services
   */
  public void startUltraSonic(String port) throws Exception {
    USfront = (UltrasonicSensor) startPeer("USfront");
    USfrontRight = (UltrasonicSensor) startPeer("USfrontRight");
    USfrontLeft = (UltrasonicSensor) startPeer("USfrontLeft");
    USback = (UltrasonicSensor) startPeer("USback");
    USbackRight = (UltrasonicSensor) startPeer("USbackRight");
    USbackLeft = (UltrasonicSensor) startPeer("USbackLeft");

    USfront.attach(arduino, frontUltrasonicTrig, frontUltrasonicEcho);
    USfrontRight.attach(arduino, front_rightUltrasonicTrig, front_rightUltrasonicEcho);
    USfrontLeft.attach(arduino, front_leftUltrasonicTrig, front_leftUltrasonicEcho);
    USback.attach(arduino, backUltrasonicTrig, backUltrasonicEcho);
    USbackRight.attach(arduino, back_rightUltrasonicTrig, back_rightUltrasonicEcho);
    USbackLeft.attach(arduino, back_leftUltrasonicTrig, back_leftUltrasonicEcho);
  }

  /**
   * Stop the tracking services
 * @throws Exception 
   */
  public void stopTrack() throws Exception {
    leftTracker.opencv.stopCapture();
    rightTracker.opencv.stopCapture();
    leftTracker.releaseService();
    rightTracker.releaseService();
    arduino.servoAttach(neck, 39);
    arduino.servoAttach(leftEye, 40);
    arduino.servoAttach(rightEye, 42);

    saying("the tracking if stopped.");
  }

  public OpenNi startOpenNI() throws Exception {
    /*
     * Start the Kinect service
     */
    if (openni == null) {
      System.out.println("starting kinect");
      openni = (OpenNi) startPeer("openni");
      pid = (Pid) startPeer("pid");

      pid.setMode("kinect", Pid.MODE_AUTOMATIC);
      pid.setOutputRange("kinect", -1, 1);
      pid.setPID("kinect", 10.0, 0.0, 1.0);
      pid.setControllerDirection("kinect", 0);

      // re-mapping of skeleton !
      // openni.skeleton.leftElbow.mapXY(0, 180, 180, 0);
      openni.skeleton.rightElbow.mapXY(0, 180, 180, 0);

      // openni.skeleton.leftShoulder.mapYZ(0, 180, 180, 0);
      openni.skeleton.rightShoulder.mapYZ(0, 180, 180, 0);

      openni.skeleton.leftShoulder.mapXY(0, 180, 180, 0);
      // openni.skeleton.rightShoulder.mapXY(0, 180, 180, 0);

      openni.addListener("publishOpenNIData", this.getName(), "onOpenNIData");
      // openni.addOpenNIData(this);
    }
    return openni;
  }

  public boolean copyGesture(boolean b) throws Exception {
    log.info("copyGesture {}", b);
    if (b) {
      if (openni == null) {
        openni = startOpenNI();
      }
      System.out.println("copying gestures");
      openni.startUserTracking();
    } else {
      System.out.println("stop copying gestures");
      if (openni != null) {
        openni.stopCapture();
        firstSkeleton = true;
      }
    }

    copyGesture = b;
    return b;
  }

  public String captureGesture() {
    return captureGesture(null);
  }

  public String captureGesture(String gestureName) {
    StringBuffer script = new StringBuffer();

    String indentSpace = "";

    if (gestureName != null) {
      indentSpace = "  ";
      script.append(String.format("def %s():\n", gestureName));
    }

    script.append(indentSpace);
    script.append(
        String.format("Sweety.setRightArmPosition(%d,%d,%d,%d,%d)\n", rightShoulder.getPos(), rightArm.getPos(), rightForearm.getPos(), rightWrist.getPos(), rightHand.getPos()));
    script.append(indentSpace);
    script
        .append(String.format("Sweety.setLeftArmPosition(%d,%d,%d,%d,%d)\n", leftShoulder.getPos(), leftArm.getPos(), leftForearm.getPos(), leftWrist.getPos(), leftHand.getPos()));
    script.append(indentSpace);
    script.append(String.format("Sweety.setHeadPosition(%d,%d,%d)\n", neck.getPos(), leftEye.getPos(), rightEye.getPos()));

    send("python", "appendScript", script.toString());

    return script.toString();
  }

  public void onOpenNIData(OpenNiData data) {

    Skeleton skeleton = data.skeleton;

    if (firstSkeleton) {
      System.out.println("i see you");
      firstSkeleton = false;
    }
    // TODO correct angles for shoulders

    int LforeArm = Math.round(skeleton.leftElbow.getAngleXY()) - (180 - leftForearmMax);
    int Larm = Math.round(skeleton.leftShoulder.getAngleXY()) - (180 - leftArmMax);
    int Lshoulder = Math.round(skeleton.leftShoulder.getAngleYZ()) + leftShoulderMin;
    int RforeArm = Math.round(skeleton.rightElbow.getAngleXY()) + rightForearmMin;
    int Rarm = Math.round(skeleton.rightShoulder.getAngleXY()) + rightArmMin;
    int Rshoulder = Math.round(skeleton.rightShoulder.getAngleYZ()) - (180 - rightShoulderMax);

    // Move the left side
    setLeftArmPosition(Lshoulder, Larm, LforeArm, -1, -1);

    // Move the left side
    setRightArmPosition(Rshoulder, Rarm, RforeArm, -1, -1);
  }

  /**
   * This static method returns all the details of the class without it having
   * to be constructed. It has description, categories, dependencies, and peer
   * definitions.
   * 
   * @return ServiceType - returns all the data
   * 
   */
  static public ServiceType getMetaData() {

    ServiceType meta = new ServiceType(Sweety.class.getCanonicalName());
    meta.addDescription("Service for the Sweety robot");
    meta.addCategory("robot");

    // put peer definitions in
    meta.addPeer("arduino", "Arduino", "arduino");
    meta.addPeer("mouth", "AcapelaSpeech", "sweetys mouth");
    meta.addPeer("ear", "WebkitSpeechRecognition", "ear");
    meta.addPeer("chatBot", "ProgramAB", "chatBot");
    meta.addPeer("leftTracker", "Tracking", "leftTracker");
    meta.addPeer("rightTracker", "Tracking", "rightTracker");
    meta.addPeer("htmlFilter", "HtmlFilter", "htmlfilter");
    meta.addPeer("webGui", "WebGui", "webGui");

    meta.addPeer("USfront", "UltrasonicSensor", "USfront");
    meta.addPeer("USfrontRight", "UltrasonicSensor", "USfrontRight");
    meta.addPeer("USfrontLeft", "UltrasonicSensor", "USfrontLeft");
    meta.addPeer("USback", "UltrasonicSensor", "USback");
    meta.addPeer("USbackRight", "UltrasonicSensor", "USbackRight");
    meta.addPeer("USbackLeft", "UltrasonicSensor", "USbackLeft");

    meta.addPeer("leftForearm", "Servo", "servo");
    meta.addPeer("rightForearm", "Servo", "servo");
    meta.addPeer("rightShoulder", "Servo", "servo");
    meta.addPeer("leftShoulder", "Servo", "servo");
    meta.addPeer("rightArm", "Servo", "servo");
    meta.addPeer("neck", "Servo", "servo");
    meta.addPeer("leftEye", "Servo", "servo");
    meta.addPeer("leftArm", "Servo", "servo");
    meta.addPeer("rightEye", "Servo", "servo");
    meta.addPeer("rightHand", "Servo", "servo");
    meta.addPeer("rightWrist", "Servo", "servo");
    meta.addPeer("leftHand", "Servo", "servo");
    meta.addPeer("leftWrist", "Servo", "servo");
    meta.addPeer("openni", "OpenNi", "openni");
    meta.addPeer("pid", "Pid", "pid");

    return meta;
  }

}
