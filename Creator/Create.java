
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.OutputStream;
import java.io.InputStream;
import java.util.Scanner;

public class Create {
    private String dir;
    public Create(String dir) {
        if (dir == null)
            dir = ".";
        this.dir = dir;
    }

    private String getProjName() {
        String projName = null;
        do {
            System.out.println("请输入新工程名称:");
            Scanner scanner = new Scanner(System.in);
            projName = scanner.nextLine();
            if (projName != null 
                && !projName.trim().equals("")
                && Character.isLetter(projName.charAt(0))
                && projName.length() >= 2 && projName.length() <= 12) {
                    String first = projName.charAt(0) + "";
                    projName = projName.replaceFirst(first, first.toUpperCase());
                break;
            }
            System.out.println("工程名称不符合规定,不能为空,且首位必须是字母,长度2-12");
        }while (true);
        return projName;
    }

    public void createProject() {
        String projName = getProjName();
        System.out.println("即将创建工程: " + projName);

        File src = new File(dir + "/Template");
        File dest = new File(dir + "/" + projName);

        System.out.println(src.exists());

        xcopy(src, dest);

        System.out.println("重命名: Template.cfg");
        new File(dest, "Template.cfg").renameTo(
            new File(dest, projName +".cfg"));
        System.out.println("重命名: Template.udf");
        new File(dest, "Template.udf").renameTo(
            new File(dest, projName +".udf"));
        System.out.println("重命名: proj.win32/Template.vcxproj");
        new File(dest, "proj.win32/Template.vcxproj").renameTo(
            new File(dest, "/proj.win32/" + projName +".vcxproj"));
        System.out.println("重命名: proj.win32/Template.vcxproj.filters");
        new File(dest, "proj.win32/Template.vcxproj.filters").renameTo(
            new File(dest, "proj.win32/" + projName +".vcxproj.filters"));
        System.out.println("重命名: proj.win32/Template.vcxproj.user");
        new File(dest, "proj.win32/Template.vcxproj.user").renameTo(
            new File(dest, "proj.win32/" + projName +".vcxproj.user"));
        
        System.out.println("修改: proj.win32/Template.sln");
        moveAndReplace(new File(dest, "proj.win32/Template.sln"), 
            new File(dest, "proj.win32/" + projName + ".sln"),
            "Template", projName);
        System.out.println("修改: Template.ccs");
        moveAndReplace(new File(dest, "Template.ccs"), 
            new File(dest, projName + ".ccs"),
            "Template", projName);

        System.out.println("修改: Classes/AppDelegate.cpp");
        moveAndReplace(new File(dest, "Classes/AppDelegate.cpp"), 
            new File(dest, "Classes/AppDelegate.cpp1"),
            "Template", projName);
        new File(dest, "Classes/AppDelegate.cpp1").renameTo(
            new File(dest, "Classes/AppDelegate.cpp"));
        System.out.println("创建成功!!!!");
    }

    private void moveAndReplace(File src, File dest, String srcS, String destS) {
        if (dest.exists()) dest.delete();

        BufferedReader br = null;
        BufferedWriter bw = null;
        try {
            br = new BufferedReader(new FileReader(src));
            bw = new BufferedWriter(new FileWriter(dest));
            String line = null;
            while( (line = br.readLine()) != null ) {
                line = line.replaceAll(srcS, destS);
                bw.write(line + "\n");
            }
            bw.flush();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (br != null)
                    br.close();
                if (bw != null)
                    bw.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        src.delete();
    }

    private boolean xcopy(File src, File dest) {
        if (!src.exists()) return false;
        if (dest.exists()) return false;        

        if (src.isDirectory()) {
            System.out.println("复制目录: " + src.getName());
            dest.mkdir();
            File[] files = src.listFiles();
            for (File f : files) {
                if (!xcopy(f, new File(dest, f.getName()))) {
                    return false;
                }
            }
        } else {
            System.out.println("复制文件: " + src.getName());
            InputStream in = null;
            OutputStream out = null;
            boolean success = true;
            try {
                if(!dest.createNewFile()) return false;

                in = new FileInputStream(src);
                out = new FileOutputStream(dest);
                byte[] buf = new byte[1024];
                int len = 0;
                while((len = in.read(buf)) != -1)
                    out.write(buf, 0, len);
                out.flush();
            } catch (Exception e) {
                e.printStackTrace();
                success = false;
            } finally {
                try {
                    if (in != null)
                        in.close();    
                    if (out != null)
                        out.close();    
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            return success;
        }

        return true;
    }


    public static void main(String[] args) {
        String dir = null;
        if (args.length > 0)
            dir = args[0];
    
        new Create(dir).createProject();
    }


}