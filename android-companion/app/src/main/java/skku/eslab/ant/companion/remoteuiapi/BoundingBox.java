package skku.eslab.ant.companion.remoteuiapi;

public class BoundingBox {
    public BoundingBox(float xMin, float yMin, float xMax, float yMax, String labelText) {
        this.xMin = xMin;
        this.yMin = yMin;
        this.xMax = xMax;
        this.yMax = yMax;
        this.labelText = labelText;
    }

    public float xMin;
    public float yMin;
    public float xMax;
    public float yMax;
    public String labelText;
}
